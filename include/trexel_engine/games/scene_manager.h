#pragma once

#include "IRenderable.h"
#include "game_actions.h"
#include "input_state.h"
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
        m_factories[std::move(id)] = std::move(factory);
    }

    // Tear down everything, we need to load one scene at a time
    void switchTo(const std::string& id);

    void push(const std::string& id);

    void pop();

    void update(float dt, const InputState& input);

    std::vector<const IRenderable*> getRenderables() const;

    Scene* current() const { return m_stack.empty() ? nullptr : m_stack.back().get(); }

private:
    void pushInternal(const std::string& id);

    std::unordered_map<std::string, Factory> m_factories;
    std::vector<std::unique_ptr<Scene>> m_stack;
};
