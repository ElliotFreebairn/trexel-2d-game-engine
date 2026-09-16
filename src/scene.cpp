#include "scene.h"
#include "game_object.h"
#include "trexel/scene_object.h"
#include <vector>

void Scene::update(float dt)
{
    for (auto& obj : mObjects)
    {
        if (obj->getIsActive())
        {
            obj->update(dt);
        }
    }

    pruneInactive();
}

std::vector<const IRenderable*> Scene::getRenderables() const
{
    std::vector<const IRenderable*> out;
    out.reserve(mObjects.size());

    for (auto& obj : mObjects)
    {
        if (obj->getIsVisible())
        {
            out.push_back(obj.get());
        }
    }
    return out;
}

void Scene::pruneInactive()
{
    mObjects.erase(
        std::remove_if(mObjects.begin(), mObjects.end(), [](const auto& o) { return !o->getIsActive(); }),
        mObjects.end());
}
