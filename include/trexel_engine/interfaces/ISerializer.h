#pragma once

#include "../nlohmann/json.hpp"

/*
 * For now this is specific to JSON but this could be the base class a JSON Serializer realises from
 */
class ISerializer
{
public:
    virtual ~ISerializer() = default;

    virtual nlohmann::json serialize() const = 0;

    virtual void deSerialize(const nlohmann::json& j) = 0;
};
