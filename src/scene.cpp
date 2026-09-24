#include "scene.h"
#include "collision.h"
#include "entity.h"
#include "game_object.h"
#include "property.h"
#include <vector>

void Scene::update(float dt)
{
    for (auto& obj : m_objects)
    {
        if (obj->isActive())
        {
            obj->setPrevPosition(obj->getPosition());
            obj->update(dt);
        }
    }

    pruneInactive();
}

void Scene::resolveCollisions(GameObject &mover)
{
    if (!mover.hasCollider()) return;

    for (auto& objPtr : m_objects)
    {
        GameObject* other = objPtr.get();
        if (other == &mover) continue;
        if (!other->hasCollider() || !other->getCollider()->isSolid) continue;

        AABB moverBounds = mover.getBounds();
        AABB otherBounds = other->getBounds();

        if (!aabbOverlap(moverBounds, otherBounds)) continue;

        glm::vec2 prevPos = mover.getPrevPosition();
        glm::vec2 currPos = mover.getPosition();

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
        mover.setPosition(pos);
        mover.evCollided(resolveX ? CollisionAxis::X : CollisionAxis::Y);
    }
}

std::vector<const IRenderable*> Scene::getRenderables() const
{
    std::vector<const IRenderable*> out;
    out.reserve(m_objects.size());

    for (auto& obj : m_objects)
    {
        if (obj->isVisible())
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
        if (!obj->isActive() || !obj->isVisible()) continue;

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
