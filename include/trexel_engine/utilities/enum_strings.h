#pragma once
#include <array>
#include <optional>
#include <string_view>
#include <utility>

// each enum specialises this to give its own table
template <typename E>
struct EnumStrings;

template<typename E>
constexpr std::string_view toString(E value)
{
    for (auto [e, name] : EnumStrings<E>::entries)
    {
        if (e == value) return name;
    }
    return "UNKNOWN";
}

template <typename E>
constexpr std::optional<E> fromString(std::string_view text)
{
    for (auto [e, name] : EnumStrings<E>::entries)
    {
        if (name == text) return e;
    }
    return std::nullopt; // no valid enum
}
