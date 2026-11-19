#include "scene.h"
#include "collision.h"
#include "entity.h"
#include "game_object.h"
#include "game_object_def.h"
#include "input_state.h"
#include "player.h"
#include "property.h"
#include "scene_object.h"
#include <cmath>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace
{
    constexpr float GRAVITY = 1500.0f;
    constexpr float MAX_FALL_SPEED = 1000.0f;
};

void Scene::update(float dt, const InputState& input)
{
    m_inUpdate = true;

    // 1. think -> objects set velocity, tick timers, spawn things etc...
    static const InputState NO_INPUT{};
    for (auto& obj : m_objects)
    {
        if (!obj->isActive()) continue;
        const FrameContext ctx {dt, obj->entityId() == m_controlled ? input : NO_INPUT, *this};
        obj->update(ctx);
    }

    // 2. physics -> only place where we move objects
    for (auto& obj : m_objects)
    {
        if (obj->isActive()) integrate(*obj, dt);
    }

    // 3. triggers -> non-blocking overlaps (arrows, future pickups, hitboxes etc...)
    reportOverlaps();

    // 4. animate
    for (auto& obj : m_objects)
    {
        if (!obj->isActive() || !obj->visual.sprite) continue;
        obj->visual.sprite->play(obj->animationState());
        obj->visual.sprite->update(dt);
    }

    m_inUpdate = false;
    flushPending();
    pruneInactive();
}

void Scene::integrate(GameObject &obj, float dt)
{
    Body& body = obj.body;
    if (body.gravity)
        body.velocity.y = std::min(body.velocity.y + GRAVITY * dt, MAX_FALL_SPEED);

    body.grounded = false; // decided every frame in moveAxis
    moveAxis(obj, {body.velocity.x * dt, 0.0f});
    moveAxis(obj, {0.0f, body.velocity.y * dt});
}

void Scene::moveAxis(GameObject &obj, glm::vec2 delta)
{
    if (delta == glm::vec2(0.0f)) return;
    obj.transform.position += delta;

    // triggers aren't pushed. they find about overlaps through reportOverlaps
    if (!obj.collider || !obj.collider->isSolid) return;

    for (auto& otherPtr : m_objects)
    {
        GameObject& other = *otherPtr;
        if (&other == &obj || !other.isActive()) continue;
        if (!other.collider || !other.collider->isSolid) continue;

        const AABB a = obj.getBounds();
        const AABB b = other.getBounds();
        if (!aabbIntersects(a, b)) continue;

        // we moved along exactly one axis, so the side we hit is known
        CollisionInfo hit {.other = &other};
        if (delta.x > 0)        { obj.transform.position.x -= a.max.x - b.min.x; hit.normal = {-1, 0}; }
        else if (delta.x < 0)   { obj.transform.position.x += b.max.x - a.min.x; hit.normal = {1, 0};  }
        else if (delta.y > 0)   { obj.transform.position.y -= a.max.y - b.min.y; hit.normal = {0, -1}; obj.body.grounded = true; }
        else                    { obj.transform.position.y += b.max.y - a.min.y; hit.normal = {0, 1};  }

        (hit.normal.x != 0 ? obj.body.velocity.x : obj.body.velocity.y) = 0.0f;
        obj.evCollided(hit);
    }
}

void Scene::reportOverlaps()
{
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        for (size_t j = i + 1; j < m_objects.size(); j++)
        {
            GameObject& a = *m_objects[i];
            GameObject& b = *m_objects[j];
            if (!a.isActive() || !b.isActive() || !a.collider || !b.collider) continue;
            if (a.collider->isSolid && b.collider->isSolid) continue; // physics already handled this
            if (!aabbIntersects(a.getBounds(), b.getBounds())) continue;

            a.evCollided({.other = &b});
            b.evCollided({.other = &a});
        }
    }
}

GameObject& Scene::addObject(std::unique_ptr<GameObject> obj)
{
    GameObject& ref = *obj;
    (m_inUpdate ? m_pending : m_objects).push_back(std::move(obj));
    return ref;
}

void Scene::flushPending()
{
    for (auto& o : m_pending) m_objects.push_back(std::move(o));
    m_pending.clear();
}

GameObject* Scene::spawn(const std::string& defName, glm::vec2 position)
{
    std::unique_ptr<GameObject> obj = m_factory.create(defName, position);
    if (!obj) return nullptr;
    return &addObject(std::move(obj));
}


std::vector<const IRenderable*> Scene::getRenderables() const
{
    std::vector<const IRenderable*> out;
    out.reserve(m_objects.size());

    for (auto& obj : m_objects)
    {
        if (obj->visual.visible)
        {
            out.push_back(obj.get());
        }
    }
    return out;
}

void Scene::pruneInactive()
{
    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(), [](const auto& o) { return !o->isActive(); }),
        m_objects.end());
}

std::vector<EntityInfo> Scene::getEntities() const
{
    std::vector<EntityInfo> out;
    out.reserve(m_objects.size());

    for (const auto& obj : m_objects)
    {
        if (!obj->isActive()) continue;

        out.push_back({obj->entityId(), obj->typeName() + " #" + std::to_string(obj->entityId())});
    }
    return out;
}

IInspectable* Scene::findInspectable(EntityId id) const
{
    if (id == INVALID_ENTITY) return nullptr;

    for (const auto& obj : m_objects)
    {
        if (obj->entityId() == id) return obj.get();
    }
    return nullptr;
}

EntityId Scene::pick(glm::vec2 worldPos) const
{
    // getRenderables() draws in list order, so the last object is on top.
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it)
    {
        GameObject* obj = it->get();
        if (!obj->isActive() || !obj->visual.visible) continue;

        // Return the closest object to the user mouse position
        const glm::vec2 pos  = obj->getPosition();
        const glm::vec2 size = obj->getSize();
        if (worldPos.x >= pos.x && worldPos.x < pos.x + size.x &&
            worldPos.y >= pos.y && worldPos.y < pos.y + size.y)
        {
            return obj->entityId();
        }
    }
    return INVALID_ENTITY;
}

void Scene::destroy(EntityId id)
{
    std::erase_if(m_objects, [id](const auto& obj) { return obj->entityId() == id; });
}
