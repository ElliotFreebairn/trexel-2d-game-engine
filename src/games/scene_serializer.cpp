#include "scene_serializer.h"
#include "game_object.h"
#include "scene.h"
#include "json_utility.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void SceneSerializer::save(const Scene& scene, const std::string& path)
{
    json out = json::object();
    out["objects"] = json::array();

    for (const auto& obj : scene.getObjects())
    {
        out["objects"].push_back(obj->serialize());
    }

    std::ofstream file(path);
    file << out.dump(2);
}

bool SceneSerializer::load(Scene& scene, const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "SceneSerializer::load: couldn't open " << path << "\n";
        return false;
    }

    json saved = json::parse(file, nullptr, /*allow exceptions*/ false);
    if (saved.is_discarded() || !saved.contains("objects"))
    {
        std::cerr << "SceneSerializer::load: " << path << " isn't valid or has no \"objects\"\n";
        return false;
    }

    for (const auto& objJson : saved["objects"])
    {
        const std::string defName = objJson.value("def", "");
        const glm::vec2 position = objJson.contains("position") ? vec2FromJson(objJson.at("position")) : glm::vec2(0.0f);

        GameObject* obj = scene.spawn(defName, position);
        if (!obj)
        {
            std::cerr << "SceneSerializer::load: skipping " << objJson << "\n";
            continue;
        }

        obj->deSerialize(objJson);
    }
    return true;
}
