#pragma once

#include <cstdint>
#include <string>

using EntityId = std::uint32_t;
constexpr EntityId INVALID_ENTITY = 0; // 0 means no "entity"

inline EntityId allocateEntityId()
{
    static EntityId next = 1;
    return next++;
}

struct EntityInfo
{
    EntityId id = INVALID_ENTITY;
    std::string name;
};
