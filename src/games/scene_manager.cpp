#include "scene_manager.h"
#include "input_state.h"
#include <vector>

void SceneManager::switchTo(const std::string &id)
{
    while (!m_stack.empty())
    {
        m_stack.back()->onExit();
        m_stack.pop_back();
    }
    pushInternal(id);
}

void SceneManager::push(const std::string &id)
{
    if (!m_stack.empty())
    {
        m_stack.back()->onPause();
    }
    pushInternal(id);
}

void SceneManager::pop()
{
    if (m_stack.empty()) return;
    m_stack.back()->onExit();
    m_stack.pop_back();
    if (!m_stack.empty())
    {
        m_stack.back()->onResume();
    }
}

void SceneManager::update(float dt, const InputState& input)
{
    if (!m_stack.empty())
    {
        m_stack.back()->update(dt, input);
    }
}

std::vector<RenderCommand> SceneManager::getRenderables() const
{
    // draw bottom-up
    std::vector<RenderCommand> out;
    for (auto& scene : m_stack)
    {
        std::vector<RenderCommand> cmds = scene->getRenderables();
        out.insert(out.end(), std::make_move_iterator(cmds.begin()), std::make_move_iterator(cmds.end()));
    }
    return out;
}

void SceneManager::pushInternal(const std::string &id)
{
    auto it = m_factories.find(id);
    assert(it != m_factories.end() && "Scene not registered");
    auto scene = it->second();
    scene->onEnter();
    m_stack.push_back(std::move(scene));
}
