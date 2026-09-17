#include "scene.h"
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
