#pragma once

#include "ability.h"

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

/*
 * Maps a name used in JSON ("bow") to a way of building an ability
 */

class AbilityRegistry
{
public:
    using Maker = std::function<std::unique_ptr<IAbility>()>;

    template <typename T>
    void registerType(const std::string& name)
    {
        static_assert(std::is_base_of_v<IAbility, T>, "T must derive from IAbility");
        static_assert(std::is_default_constructible_v<T>, "abilities are built with no arguments");
        m_makers[name] = [] { return std::make_unique<T>(); };
    }

    // nullptr if nothing is registered
    const Maker* find(const std::string& name) const
    {
        auto it = m_makers.find(name);
        return it != m_makers.end() ? &it->second : nullptr;
    }
private:
    std::unordered_map<std::string, Maker> m_makers;
};
