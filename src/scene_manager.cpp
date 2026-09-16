#include "scene_manager.h"
#include "IRenderable.h"
#include <vector>

void SceneManager::switchTo(const std::string &id)
{
    while (!mStack.empty())
    {
        mStack.back()->onExit();
        mStack.pop_back();
    }
    pushInternal(id);
}

void SceneManager::push(const std::string &id)
{
    if (!mStack.empty())
    {
        mStack.back()->onPause();
    }
    pushInternal(id);
}

void SceneManager::pop()
{
    if (mStack.empty()) return;
    mStack.back()->onExit();
    mStack.pop_back();
    if (!mStack.empty())
    {
        mStack.back()->onResume();
    }
}

void SceneManager::update(float dt)
{
    if (!mStack.empty())
    {
        mStack.back()->update(dt);
    }
}

void SceneManager::handleAction(const GameAction &action)
{
    if (!mStack.empty())
    {
        mStack.back()->handleAction(action);
    }
}

std::vector<const IRenderable*> SceneManager::getRenderables() const
{
    // draw bottom-up
    std::vector<const IRenderable*> out;
    for (auto& scene : mStack)
    {
        auto renderables = scene->getRenderables();
        out.insert(out.end(), renderables.begin(), renderables.end());
    }
    return out;
}

void SceneManager::pushInternal(const std::string &id)
{
    auto it = mFactories.find(id);
    assert(it != mFactories.end() && "Scene not registered");
    auto scene = it->second();
    scene->onEnter();
    mStack.push_back(std::move(scene));
}
