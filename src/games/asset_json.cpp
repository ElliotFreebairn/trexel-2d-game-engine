#include "asset_json.h"

void to_json(nlohmann::json& j, const Asset& a)
{
    j = {{"id", a.id}, {"path", a.filePath}, {"type", a.type}};
}

void from_json(const nlohmann::json& j, Asset& a)
{
    j.at("id").get_to(a.id);
    j.at("path").get_to(a.filePath);
    j.at("type").get_to(a.type);
}
