#include "scene.h"
#include "collision.h"
#include "entity.h"
#include "game_object.h"
#include "game_object_def.h"
#include "player.h"
#include "property.h"
#include "scene_object.h"
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Scene::update(float dt)
{
    for (auto& obj : m_objects)
    {
        if (obj->isActive())
        {
            obj->update(dt);
            obj->applyGravity(dt);
        }
    }

    pruneInactive();
}

GameObject& Scene::addObject(std::unique_ptr<GameObject> obj)
{
    GameObject& ref = *obj;
    m_objects.push_back(std::move(obj));
    return ref;
}

GameObject* Scene::spawn(const std::string& defName, glm::vec2 position)
{
    std::unique_ptr<GameObject> obj = m_factory.create(defName, position);
    if (!obj) return nullptr;
    return &addObject(std::move(obj));
}

void Scene::snapshotPositions()
{
    for (auto& objPtr : m_objects)
    {
        objPtr->transform.prevPosition = objPtr->transform.position;
    }
}

void Scene::resolveCollisions(GameObject &mover)
{
    if (!mover.hasCollider()) return;

    for (auto& objPtr : m_objects)
    {
        GameObject* other = objPtr.get();
        if (other == &mover) continue;
        if (!other->hasCollider() || !other->collider->isSolid) continue;

        AABB moverBounds = mover.getBounds();
        AABB otherBounds = other->getBounds();

        if (!aabbOverlap(moverBounds, otherBounds)) continue;

        glm::vec2 prevPos = mover.transform.prevPosition;
        glm::vec2 currPos = mover.transform.position;

        // Isolate each axis: does moving *only* on that axis explain the overlap?
        bool xAlone = aabbOverlap(mover.getBoundsAt({currPos.x, prevPos.y}), otherBounds);
        bool yAlone = aabbOverlap(mover.getBoundsAt({prevPos.x, currPos.y}), otherBounds);

        float overlapX = std::min(moverBounds.max.x, otherBounds.max.x)
            - std::max(moverBounds.min.x, otherBounds.min.x);
        float overlapY = std::min(moverBounds.max.y, otherBounds.max.y)
            - std::max(moverBounds.min.y, otherBounds.min.y);

        bool resolveX;
        if (xAlone && !yAlone)      resolveX = true;   // clearly a side bump
        else if (yAlone && !xAlone) resolveX = false;  // clearly a floor/ceiling hit
        else                        resolveX = overlapX < overlapY; // ambiguous corner: fall back to old heuristic

        glm::vec2 pos = currPos;
        if (resolveX)
        {
            pos.x += (moverBounds.min.x < otherBounds.min.x) ? -overlapX : overlapX;
        }
        else
        {
            pos.y += (moverBounds.min.y < otherBounds.min.y) ? -overlapY : overlapY;
        }
        mover.transform.position = pos;
        mover.evCollided(resolveX ? CollisionAxis::X : CollisionAxis::Y);
    }
}

bool Scene::overlapsSolid(const GameObject &probe, EntityId ignore) const
{
    if (!probe.hasCollider()) return false;

    const AABB probeBounds = probe.getBounds();
    for (const auto& objPtr : m_objects)
    {
        const GameObject* other = objPtr.get();
        if (other == &probe) continue;
        if (ignore != INVALID_ENTITY && other->entityId() == ignore) continue;
        if (!other->hasCollider() || !other->collider->isSolid) continue;

        if (aabbOverlap(probeBounds, other->getBounds())) return true;
    }
    return false;
}

bool Scene::isGroundedOn(const GameObject &probe, float skinWidth, EntityId ignore)
{
    if (!probe.hasCollider()) return false;

    AABB bounds = probe.getBounds();

    bounds.min.x += 2.0f;
    bounds.max.x -= 2.0f;

    bounds.min.y += skinWidth;
    bounds.max.y += skinWidth;

    for (const auto& objPtr : m_objects)
    {
        const GameObject* other = objPtr.get();
        if (other == &probe) continue;
        if (ignore != INVALID_ENTITY && other->entityId() == ignore) continue;
        if (!other->hasCollider() || !other->collider->isSolid) continue;

        if (aabbOverlap(bounds, other->getBounds())) return true;
    }
    return false;
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
