#pragma once

#include "IRenderable.h"
#include "game_actions.h"
#include "scene.h"
#include <memory>
#include <unordered_map>
#include <vector>
class SceneManager
{
public:
    using Factory = std::function<std::unique_ptr<Scene>()>;

    void registerScene(std::string id, Factory factory)
    {
        mFactories[std::move(id)] = std::move(factory);
    }

    // Tear down everything, we need to load one scene at a time
    void switchTo(const std::string& id);

    void push(const std::string& id);

    void pop();

    void update(float dt);
    void handleAction(const GameAction& action);

    std::vector<const IRenderable*> getRenderables() const;

    Scene* current() const { return mStack.empty() ? nullptr : mStack.back().get(); }

private:
    void pushInternal(const std::string& id);

    std::unordered_map<std::string, Factory> mFactories;
    std::vector<std::unique_ptr<Scene>> mStack;
};
