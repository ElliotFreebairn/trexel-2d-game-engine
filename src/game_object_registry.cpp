#include "game_object_registry.h"
#include "game_object.h"

void GameObjectRegistry::registerType(std::string className, Factory factory)
{
    m_factories[std::move(className)] = std::move(factory);
}

std::unique_ptr<GameObject> GameObjectRegistry::create(const std::string &className, glm::vec2 position, const GameObjectDef &def) const
{
    auto it = m_factories.find(className);
    if (it == m_factories.end()) return nullptr;
    return it->second(position, def);
}

bool GameObjectRegistry::isRegistered(const std::string &className) const
{
    return m_factories.find(className) != m_factories.end();
}
