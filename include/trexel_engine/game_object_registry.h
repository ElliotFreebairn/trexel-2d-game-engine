#pragma once

#include "game_object.h"
#include "game_object_def.h"

#include <functional>
#include <memory>
#include <unordered_map>

class GameObject;

class GameObjectRegistry
{
public:
    using Factory = std::function<std::unique_ptr<GameObject>(glm::vec2 position, const GameObjectDef& def)>;

    void registerType(std::string className, Factory factory);

    std::unique_ptr<GameObject> create(const std::string& className, glm::vec2 position, const GameObjectDef& def) const;

    bool isRegistered(const std::string& className) const;

private:
    std::unordered_map<std::string, Factory> m_factories;
};
