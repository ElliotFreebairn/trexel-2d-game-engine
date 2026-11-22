#include "scene_serializer.h"
#include "ecs/components.h"
#include "json_utility.h"
#include "scene.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void SceneSerializer::save(Scene& scene, const std::string& path)
{
    Registry& reg = scene.registry();

    // Sort by id so the file keeps the same order between saves (clean git diffs)
    std::vector<EntityId> ids;
    reg.each<Prefab>([&](EntityId e, Prefab&) { ids.push_back(e); });
    std::sort(ids.begin(), ids.end());

    json out = json::object();
    out["objects"] = json::array();

    for (EntityId e : ids)
    {
        const Prefab& prefab = *reg.get<Prefab>(e);
        const Transform* t = reg.get<Transform>(e);
        if (!t) continue;
        const GameObjectDef* def = scene.factory().getDefinition(prefab.name);

        // Only save what differs from the definition
        json j = json::object();
        j["def"] = prefab.name;
        j["position"] = vec2ToJson(t->position);
        if (!def || t->size != def->transform.size) j["size"] = vec2ToJson(t->size);
        if (t->angle != 0.0) j["angle"] = t->angle;

        if (const Visual* v = reg.get<Visual>(e))
        {
            if (!v->visible) j["visible"] = false;
            if (def && v->textureId != def->visual.textureId) j["textureId"] = v->textureId;
        }
        if (const Health* h = reg.get<Health>(e); h && def && def->health && h->current != def->health->current)
        {
            j["health"] = h->current;
        }

        out["objects"].push_back(j);
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

    Registry& reg = scene.registry();
    for (const auto& objJson : saved["objects"])
    {
        const std::string defName = objJson.value("def", "");
        const glm::vec2 position = objJson.contains("position") ? vec2FromJson(objJson.at("position")) : glm::vec2(0.0f);

        EntityId e = scene.spawn(defName, position);
        if (e == INVALID_ENTITY)
        {
            std::cerr << "SceneSerializer::load: skipping " << objJson << "\n";
            continue;
        }

        // Apply per-entity overrides on top of the definition
        Transform& t = *reg.get<Transform>(e); // spawn always adds a Transform
        if (objJson.contains("size")) t.size = vec2FromJson(objJson.at("size"));
        t.angle = objJson.value("angle", t.angle);

        if (Visual* v = reg.get<Visual>(e))
        {
            v->visible = objJson.value("visible", v->visible);
            v->textureId = objJson.value("textureId", v->textureId);
        }
        if (Health* h = reg.get<Health>(e))
        {
            h->current = objJson.value("health", h->current);
        }
    }
    return true;
}
