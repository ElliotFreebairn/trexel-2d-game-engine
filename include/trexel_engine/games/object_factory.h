#pragma once

#include "ability_registry.h"
#include "ecs/registry.h"
#include "game_object_def.h"

#include <string>
#include <unordered_map>
#include <vector>

/*
 * Loads definitions (data/definitions/objects.json) and spawns entities from them
 *
 * A definition describes an entity by the components it has, abilities named in JSON
 * must be registered first:
 *      factory.registerAbility<BowAbility>("bow");
 *
 * Then any definition can be spawned by name:
 *      factory.spawn(registry, "archer", position);
 */
class ObjectFactory
{
public:
    template <typename T>
    void registerAbility(const std::string& name)
    {
        m_abilities.registerType<T>(name);
    }

    bool loadDefinitions(const std::string& path);

    bool saveDefinitions(const std::string& path);

    bool addDefinition(const GameObjectDef& def);

    // nullptr if there is no definition with that name
    const GameObjectDef* getDefinition(const std::string& name) const;

    // builds the entity in the registry, return INVALID_ENTITY if failire
    EntityId spawn(Registry& reg, const std::string& defName, glm::vec2 position) const;

    std::vector<std::string> getDefinitionsKeys() const;

    std::vector<GameObjectDef> getDefinitionsValues() const;

private:
    // throws if a definition names an ability that isn't registered
    void resolveAbility(GameObjectDef& def) const;

    std::unordered_map<std::string, GameObjectDef> m_definitions;
    AbilityRegistry m_abilities;
};
