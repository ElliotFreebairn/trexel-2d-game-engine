#pragma once

#include "game_object.h"
#include "game_object_def.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * Creates game objects from their definitions (assets/definitions/objects.json)
 *
 * Each definition says which class it builds, e.g. "class": "Player", that class must be registered first:
 *      factory.registerType<Player>("Player");
 *
 * Then any definition can be built by name:
 *      factory.create("archer", position);
 */
class ObjectFactory
{
public:
    using Constructor = std::function<std::unique_ptr<GameObject>(const GameObjectDef& def, glm::vec2 position)>;

    template <typename T>
    void registerType(const std::string& className)
    {
        static_assert(std::is_base_of_v<GameObject, T>, "T must derive from game object");
        m_constructors[className] = [](const GameObjectDef& def, glm::vec2 position) {
            return std::make_unique<T>(def, position);
        };
    }

    bool loadDefinitions(const std::string& path);

    bool saveDefinitions(const std::string& path);

    bool addDefinition(const GameObjectDef& def);

    // nullptr if there is no definition with that name
    const GameObjectDef* getDefinition(const std::string& name) const;

    // nullptr if the definition or its class doesn't exist
    std::unique_ptr<GameObject> create(const std::string& defName, glm::vec2 position) const;

    // expose a public facing function to get the different defintions via id
    std::vector<std::string> getDefinitionsKeys() const;

    // expose a public facing function to get the actual game object definitios
    std::vector<GameObjectDef> getDefinitionsValues() const;

private:
    std::unordered_map<std::string, GameObjectDef> m_definitions;
    std::unordered_map<std::string, Constructor> m_constructors;
};
