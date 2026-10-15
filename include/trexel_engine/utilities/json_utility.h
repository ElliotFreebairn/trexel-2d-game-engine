#pragma once

/*
 * Maybe this shoudn't be JSON Utility, as it needs access to glm::vec2
 */

#include "../nlohmann/json.hpp"
#include <glm/glm.hpp>
using json = nlohmann::json;

namespace
{
json vec2ToJson(glm::vec2 v)
{
    json j = json::object();
    j["x"] = v.x;
    j["y"] = v.y;
    return j;
}

glm::vec2 vec2FromJson(const json& j)
{
    return glm::vec2(j.at("x").get<float>(), j.at("y").get<float>());
}

json vec3ToJson(glm::vec3 v)
{
    json j = json::object();
    j["r"] = v.x;
    j["g"] = v.y;
    j["b"] = v.z;
    return j;
}

glm::vec3 vec3FromJson(const json& j)
{
    return glm::vec3(j.at("r").get<float>(), j.at("g").get<float>(), j.at("b").get<float>());
}
}
