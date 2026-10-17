#include "object_factory.h"
#include "collision.h"
#include "game_object_def.h"
#include "json_utility.h"
#include "sprite.h"

#include <fstream>
#include <iostream>

/*
 * Object Definition JSON Example
 *
 * {
 *  "definitions":
 *      [
 *          {
 *              "name" : string, (example "archer")
 *              "class" : string, (the registered C++ class to build, default "SceneObject")
 *              "textureId" : string, (example "archer_idle", if the animations are defined the texture is unused)
 *              "size": glm::vec2, (the size of the image)
 *              "gravity": bool, (default false)
 *              "collider" : (optional)
 *                  {
 *                      "size" : glm::vec2,  (default is the object size)
 *                      "offset": glm::vec2, (default {0, 0}),
 *                      "solid": bool (default true)
 *                  },
 *               "animations": (optional, states must match the SpriteMovementState enum), (frame size = size)
 *                {
 *                  "frameSpeed" : float, (default 0.2f)
 *                  "states" : [
 *                      {
 *                          "state" : "IDLE",
 *                          "textureId" : string,
 *                          "frames" : x,
 *                          "rows" : x, (default to 1),
 *                          "type" : "SINGULAR" etc.. (default is continous)
 *                      }
 *                  ]
 *                }
 *          }
 *      ]
 * }
 */

using json = nlohmann::json;

namespace
{
GameObjectDef parseDefinition(const json& defJson)
{
    GameObjectDef def;
    def.name = defJson.at("name").get<std::string>();
    def.className = defJson.value("class", def.className);
    def.textureId = defJson.value("textureId", "");
    def.size = vec2FromJson(defJson.at("size"));
    def.gravity = defJson.value("gravity", false);

    if (defJson.contains("collider"))
    {
        const json& cJson = defJson.at("collider");

        Collider collider;
        collider.size = cJson.contains("size") ? vec2FromJson(cJson.at("size")) : def.size;
        collider.offset = cJson.contains("offset") ? vec2FromJson(cJson.at("offset")) : glm::vec2(0.0f);
        collider.isSolid = cJson.value("solid", true);

        def.collider = collider;
    }

    if (defJson.contains("animations"))
    {
        const json& aJson = defJson.at("animations");
        const float frameSpeed = aJson.value("frameSpeed", 0.2f);

        for (const auto& animation : aJson.at("states"))
        {
            SpriteAnimation spriteAnimation;
            spriteAnimation.textureID = animation.at("textureId").get<std::string>();
            spriteAnimation.columns = animation.at("frames").get<int>();
            spriteAnimation.rows = animation.value("rows", 1);
            spriteAnimation.animationType = Sprite::toAnimationType(animation.value("type", "CONTINOUS"));

            // Each frame is the size of the object
            spriteAnimation.frameWidth = static_cast<int>(def.size.x);
            spriteAnimation.frameHeight = static_cast<int>(def.size.y);

            // Each frame is usally defined by the generic frame speed, but some may override...
            spriteAnimation.frameDuration = animation.value("frameSpeed", frameSpeed);

            SpriteMovementState state = Sprite::toSpriteMovementState(animation.at("state").get<std::string>());
            def.animations[state] = spriteAnimation;
        }
    }

    return def;
}
}

bool ObjectFactory::loadDefinitions(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ObjectFactory::loadDefinitions: cannot open " << path << "\n";
        return false;
    }

    json loaded = json::parse(file, nullptr, /*allow exceptions*/ false);
    if (loaded.is_discarded() || !loaded.contains("definitions"))
    {
        std::cerr << "ObjectFactory::loadDefinitions: " << path << " isn't valid or has no \"definitions\"\n";
        return false;
    }

    for (const auto& defJson : loaded["definitions"])
    {
        try
        {
            GameObjectDef def = parseDefinition(defJson);
            m_definitions[def.name] = def;
        }
        catch (const json::exception& e)
        {
            std::cerr << "ObjectFactory::loadDefinitions: skipping a definition, " << e.what() << "\n";
        }
    }
    return true;
}

const GameObjectDef* ObjectFactory::getDefinition(const std::string& name) const
{
    auto it = m_definitions.find(name);
    return it != m_definitions.end() ? &it->second : nullptr;
}

std::unique_ptr<GameObject> ObjectFactory::create(const std::string& defName, glm::vec2 position) const
{
    const GameObjectDef* def = getDefinition(defName);
    if (!def)
    {
        std::cerr << "ObjectFactory::create: no definition called \"" << defName << "\"\n";
        return nullptr;
    }

    auto it = m_constructors.find(def->className);
    if (it == m_constructors.end())
    {
        std::cerr << "ObjectFactory::create: class \"" << def->className << "\" isn't registered\n";
        return nullptr;
    }

    return it->second(*def, position);
}
