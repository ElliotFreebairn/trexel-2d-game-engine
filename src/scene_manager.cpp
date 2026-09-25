#include "scene_manager.h"
#include "IRenderable.h"
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

void SceneManager::update(float dt)
{
    if (!m_stack.empty())
    {
        m_stack.back()->update(dt);
    }
}

std::vector<const IRenderable*> SceneManager::getRenderables() const
{
    // draw bottom-up
    std::vector<const IRenderable*> out;
    for (auto& scene : m_stack)
    {
        auto renderables = scene->getRenderables();
        out.insert(out.end(), renderables.begin(), renderables.end());
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
