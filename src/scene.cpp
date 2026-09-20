#include "scene.h"
#include "collision.h"
#include "game_object.h"
#include <vector>

void Scene::update(float dt)
{
    for (auto& obj : m_objects)
    {
        if (obj->isActive())
        {
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

        float overlapX = std::min(moverBounds.max.x, otherBounds.max.x)
            - std::max(moverBounds.min.x, otherBounds.min.x);
        float overlapY = std::min(moverBounds.max.y, otherBounds.max.y)
            - std::max(moverBounds.min.y, otherBounds.min.y);

        glm::vec2 pos = mover.getPosition();

        // Push along axis that has the smaller gap
        if (overlapX < overlapY)
        {
            pos.x += (moverBounds.min.x < otherBounds.min.x) ? -overlapX : overlapX;
        }
        else
        {
            pos.y += (moverBounds.min.y < otherBounds.min.y) ? -overlapY : overlapY;
        }
        mover.setPosition(pos);
        mover.evCollided();
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
