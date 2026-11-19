#include "scene.h"

#include "ecs/components.h"
#include "ecs/systems.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <variant>

void Scene::update(float dt, const InputState& input)
{
    m_collisions.clear();

    // 1. write intent, sort of the brains
    patrolAISystem(m_reg);
    playerInputSystem(m_reg, input);

    // 2. bodie related components must act on what has been decided
    movementSystem(m_reg);
    attackSystem(*this, dt);

    // 3. the only place where positions can change!
    physicsSystem(m_reg, dt, m_collisions);

    // 4. reactions to the frames changes
    patrolBounceSystem(m_reg, m_collisions);
    projectileSystem(m_reg, dt, m_collisions);
    damageSystem(m_reg, m_collisions);
    deathSystem(m_reg);

    // 5. visual so sprites, animations etc...
    animationSystem(m_reg, dt);

    // 6. if a entity has died, we can destroy now as we are not iterating over any objects
    m_reg.flushDestroyed();
}

EntityId Scene::spawn(const std::string& defName, glm::vec2 position)
{
    return m_factory.spawn(m_reg, defName, position);
}

/*
 * Will eventually by the implemented renderer like SDL for our instance
 */
std::vector<EntityId> Scene::drawOrder()
{
    std::vector<std::pair<int, EntityId>> order; // (layer, id)
    m_reg.each<Visual, Transform>([&](EntityId e, Visual& v, Transform&) {
        if (v.visible) order.push_back({v.layer, e});
    });
    std::sort(order.begin(), order.end()); // sort by layer first then id

    std::vector<EntityId> ids;
    ids.reserve(order.size());
    for (const auto& [layer, e] : order)
    {
        ids.push_back(e);
    }
    return ids;
}

std::vector<RenderCommand> Scene::getRenderables()
{
    std::vector<RenderCommand> out;
    for (EntityId e : drawOrder())
    {
        // drawOrder guarentees that both Visual and Transform component exists
        const Transform& t = *m_reg.get<Transform>(e);
        const Visual& v = *m_reg.get<Visual>(e);

        RenderCommand cmd;
        cmd.position = t.position;
        cmd.size = t.size;
        cmd.angle = t.angle;
        cmd.flip = t.flip;
        cmd.textureId = v.sprite ? v.sprite->getTextureID() : v.textureId;
        if (v.sprite) cmd.sourceRect = v.sprite->getSrcRect();
        cmd.colour = v.colour;
        if (const Collider* c = m_reg.get<Collider>(e))
        {
            cmd.collider = worldBounds(t, c);
        }
        out.push_back(std::move(cmd));
    }
    return out;
}

EntityId Scene::pick(glm::vec2 p)
{
    const std::vector<EntityId> order = drawOrder();
    // pick the front-most entity e.g. by drawOrder
    for (auto it = order.rbegin(); it != order.rend(); ++it)
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
    // unordered_map order is arbitrary so we should atleast sort it to remain order
    std::sort(out.begin(), out.end(), [](const EntityInfo& a, const EntityInfo& b) {
        return a.id < b.id;
    });
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

    // only components that each game object entity has
    auto& props = data.properties;
    props.push_back({"position", t->position});
    props.push_back({"size", t->size});
    props.push_back({"angle", static_cast<float>(t->angle)});

    if (Visual* v = m_reg.get<Visual>(id))
    {
        props.push_back({"visible", v->visible});
        props.push_back({"texture_id", v->textureId});
        props.push_back({"layer", v->layer});
    }
    if (Body* b = m_reg.get<Body>(id))
    {
        props.push_back({"velocity", b->velocity, true});
        props.push_back({"grounded", b->grounded, true});
    }
    if (Movement* l = m_reg.get<Movement>(id))
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
        if (name == "position")
        {
            t->position = std::get<glm::vec2>(value);
        }
        else if (name == "size") {
            t->size = std::get<glm::vec2>(value);
        }
        else if (name == "angle") {
            t->angle = std::get<float>(value);
        }
    }
    if (Visual* v = m_reg.get<Visual>(id))
    {
        if (name == "visible")
        {
            v->visible = std::get<bool>(value);
        }
        else if (name == "texture_id")
        {
            v->textureId = std::get<std::string>(value);
        }
        else if (name == "layer")
        {
            v->layer = std::get<int>(value);
        }
    }
    if (Movement* l = m_reg.get<Movement>(id))
    {
        if (name == "move_speed")
        {
            l->moveSpeed = std::get<float>(value);
        }
        else if (name == "jump_force")
        {
            l->jumpForce = std::get<float>(value);
        }
    }
    if (Health* h = m_reg.get<Health>(id); h && name == "health")
    {
        h->current = std::get<int>(value);
    }
}

void Scene::destroy(EntityId id)
{
    m_reg.destroy(id);
    m_reg.flushDestroyed(); // the scene can only call this when we've stopped itearting over the reg
}

void Scene::setControlled(EntityId id)
{
    if (!m_reg.has<Intent>(id)) return; // we shoudn't drive an object that no intent

    // collect first as looping over the current player controlled will break the loop
    std::vector<EntityId> current;
    m_reg.each<PlayerControlled>([&](EntityId e, PlayerControlled&) {
        current.push_back(e);
    });

    for (EntityId e : current)
    {
        m_reg.remove<PlayerControlled>(e);
        if (Intent* intent = m_reg.get<Intent>(e)) *intent = {};
    }

    m_reg.add<PlayerControlled>(id);
}
