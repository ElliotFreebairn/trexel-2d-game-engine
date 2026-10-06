#include "scene_serializer.h"
#include "game_object.h"
#include "game_object_registry.h"
#include "object_defs.h"
#include "scene.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

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

void SceneSerializer::save(const Scene& scene, const std::string& path)
{
    json out = json::object();
    out["objects"] = json::array();

    for (const auto& objPtr : scene.getObjects())
    {
        const GameObject& obj = *objPtr;

        json objJson = json::object();
        objJson["class"] = obj.typeName();
        objJson["objectDef"] = obj.defName();
        objJson["position"] = vec2ToJson(obj.getPosition());
        objJson["size"] = vec2ToJson(obj.getSize());

        if (obj.getColour().has_value())
        {
            objJson["colour"] = vec3ToJson(*obj.getColour());
        }

        out["objects"].push_back(objJson);
    }

    std::ofstream file(path);
    file << out;
}

bool SceneSerializer::load(Scene& scene, const std::string& path, const GameObjectRegistry& registry)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "SceneSerializer::load: couldn't open " << path << "\n";
        return false;
    }

    json saved = json::parse(file);
    if (!saved.contains("objects")) return false;

    for (const auto& objJson : saved["objects"])
    {
        const std::string className = objJson.at("class").get<std::string>();
        const std::string defName = objJson.at("objectDef").get<std::string>();
        const glm::vec2 position = vec2FromJson(objJson.at("position"));

        GameObjectDef def;
        if (!defName.empty())
        {
            try
            {
                def = ObjectDefs::getGameObjectDef(defName);
            }
            catch (const std::exception& e)
            {
                std::cerr << "SceneSerializer::load: " << e.what() << ", skipping one object\n";
                std::cerr << "JSON PayLoad: \n" << saved;
                continue;
            }
        }

        std::unique_ptr<GameObject> obj = registry.create(className, position, def);
        if (!obj)
        {
            std::cerr << "SceneSerializer::load: unknown class \"" << className << "\", skipping one object\n";
            std::cerr << "JSON PayLoad: \n" << saved;
            continue;
        }

        obj->setDefName(defName);

        if (objJson.contains("size"))
        {
            obj->setSize(vec2FromJson(objJson.at("size")));
        }

        scene.addObject(std::move(obj));
    }
    return true;
}
