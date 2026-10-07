#pragma once

#include "../nlohmann/json.hpp"

/*
 * For now this is specific to JSON but this could be the base class a JSON Serializer realises from
 */
class ISerializer
{
    virtual nlohmann::json serialize();

    virtual void deSerialize(const nlohmann::json& j);
};
