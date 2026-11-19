#include "scene.h"

#include "ecs/components.h"
#include "ecs/systems.h"

#include <algorithm>
#include <utility>
#include <variant>

void Scene::update(float dt, const InputState& input)
{
    m_collisions.clear();

    // 1. Brains decide
    patrolAISystem(m_reg);
    playerInputSystem(m_reg, input);

    // 2. Bodies act on what was decided
    locomotionSystem(m_reg);
    attackSystem(*this, dt);

    // 3. Physics: the only place positions change; records collisions
    physicsSystem(m_reg, dt, m_collisions);

    // 4. Reactions to this frame's collisions
    patrolBounceSystem(m_reg, m_collisions);
    projectileSystem(m_reg, dt, m_collisions);
    damageSystem(m_reg, m_collisions);
    deathSystem(m_reg);

    // 5. Visuals
    animationSystem(m_reg, dt);

    // 6. Apply deaths now that nobody is iterating
    m_reg.flushDestroyed();
}

EntityId Scene::spawn(const std::string& defName, glm::vec2 position)
{
    return m_factory.spawn(m_reg, defName, position);
}

std::vector<EntityId> Scene::drawOrder()
{
    std::vector<std::pair<int, EntityId>> order; // (layer, id)
    m_reg.each<Visual, Transform>([&](EntityId e, Visual& v, Transform&) {
        if (v.visible) order.push_back({v.layer, e});
    });
    std::sort(order.begin(), order.end()); // pairs sort by first, then second

    std::vector<EntityId> ids;
    ids.reserve(order.size());
    for (const auto& [layer, e] : order) ids.push_back(e);
    return ids;
}

std::vector<RenderCommand> Scene::getRenderables()
{
    std::vector<RenderCommand> out;
    for (EntityId e : drawOrder())
    {
        const Transform& t = *m_reg.get<Transform>(e); // drawOrder guarantees both exist
        const Visual& v    = *m_reg.get<Visual>(e);

        RenderCommand cmd;
        cmd.position  = t.position;
        cmd.size      = t.size;
        cmd.angle     = t.angle;
        cmd.flip      = t.flip;
        cmd.textureId = v.sprite ? v.sprite->getTextureID() : v.textureId;
        if (v.sprite) cmd.sourceRect = v.sprite->getSrcRect();
        cmd.colour    = v.colour;
        if (const Collider* c = m_reg.get<Collider>(e)) cmd.collider = worldBounds(t, c);

        out.push_back(std::move(cmd));
    }
    return out;
}

EntityId Scene::pick(glm::vec2 p)
{
    const std::vector<EntityId> order = drawOrder();
    for (auto it = order.rbegin(); it != order.rend(); ++it) // front-most first
    {
        const Transform& t = *m_reg.get<Transform>(*it);
        if (p.x >= t.position.x && p.x < t.position.x + t.size.x &&
            p.y >= t.position.y && p.y < t.position.y + t.size.y)
        {
            return *it;
        }
    }
    return INVALID_ENTITY;
}

std::vector<EntityInfo> Scene::getEntities()
{
    std::vector<EntityInfo> out;
    m_reg.each<Prefab>([&](EntityId e, Prefab& p) {
        out.push_back({e, p.name + " #" + std::to_string(e)});
    });
    // unordered_map order is arbitrary, sort so the list doesn't jump around in the UI
    std::sort(out.begin(), out.end(), [](const EntityInfo& a, const EntityInfo& b) { return a.id < b.id; });
    return out;
}

std::optional<InspectorData> Scene::inspect(EntityId id)
{
    Transform* t = m_reg.get<Transform>(id);
    if (!t) return std::nullopt;

    InspectorData data;
    data.id = id;
    const Prefab* prefab = m_reg.get<Prefab>(id);
    data.name = prefab ? prefab->name : "entity";

    // Only components this entity has show up
    auto& props = data.properties;
    props.push_back({"position", t->position});
    props.push_back({"size", t->size});
    props.push_back({"angle", static_cast<float>(t->angle)});

    if (Visual* v = m_reg.get<Visual>(id))
    {
        props.push_back({"visible", v->visible});
        props.push_back({"texture_id", v->textureId});
    }
    if (Body* b = m_reg.get<Body>(id))
    {
        props.push_back({"velocity", b->velocity, true});
        props.push_back({"grounded", b->grounded, true});
    }
    if (Locomotion* l = m_reg.get<Locomotion>(id))
    {
        props.push_back({"move_speed", l->moveSpeed});
        props.push_back({"jump_force", l->jumpForce});
    }
    if (Health* h = m_reg.get<Health>(id))
    {
        props.push_back({"health", h->current});
    }

    props.push_back({"controlled", m_reg.has<PlayerControlled>(id), true});
    return data;
}

void Scene::setProperty(EntityId id, const std::string& name, const PropertyValue& value)
{
    if (Transform* t = m_reg.get<Transform>(id))
    {
        if      (name == "position") t->position = std::get<glm::vec2>(value);
        else if (name == "size")     t->size     = std::get<glm::vec2>(value);
        else if (name == "angle")    t->angle    = std::get<float>(value);
    }
    if (Visual* v = m_reg.get<Visual>(id))
    {
        if      (name == "visible")    v->visible   = std::get<bool>(value);
        else if (name == "texture_id") v->textureId = std::get<std::string>(value);
    }
    if (Locomotion* l = m_reg.get<Locomotion>(id))
    {
        if      (name == "move_speed") l->moveSpeed = std::get<float>(value);
        else if (name == "jump_force") l->jumpForce = std::get<float>(value);
    }
    if (Health* h = m_reg.get<Health>(id); h && name == "health")
    {
        h->current = std::get<int>(value);
    }
}

void Scene::destroy(EntityId id)
{
    m_reg.destroy(id);
    m_reg.flushDestroyed(); // called from the editor between frames, so nothing is iterating
}

void Scene::setControlled(EntityId id)
{
    if (!m_reg.has<Intent>(id)) return; // e.g. a grass block has nothing to drive

    // Collect first: removing PlayerControlled while looping the PlayerControlled pool breaks the loop
    std::vector<EntityId> current;
    m_reg.each<PlayerControlled>([&](EntityId e, PlayerControlled&) { current.push_back(e); });

    for (EntityId e : current)
    {
        m_reg.remove<PlayerControlled>(e);
        if (Intent* intent = m_reg.get<Intent>(e)) *intent = {}; // otherwise it keeps walking
    }

    m_reg.add<PlayerControlled>(id);
}
