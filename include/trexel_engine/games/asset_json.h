#pragma once
#include "asset.h"
#include "../nlohmann/json.hpp"

// maps the enum to strings
NLOHMANN_JSON_SERIALIZE_ENUM(AssetType, {
    {AssetType::Texture, "texture"}
})

void to_json(nlohmann::json& j, const Asset& a);
void from_json(const nlohmann::json& j, Asset& a);
