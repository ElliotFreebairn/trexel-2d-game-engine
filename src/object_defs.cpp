#include "object_defs.h"
#include "collision.h"
#include "game_object_def.h"
#include "nlohmann/json.hpp"
#include "json_utility.h"
#include "sprite.h"
#include <fstream>
#include <unordered_map>
/*
 * Object Definition JSON Example
 *
 * This is specific to how a game object looks, not its properties, that's why it is generic to the game object
 *
 * {
 *  "definitions":
 *      [
 *          {
 *              "name" : string, (example "ARCHER")
 *              "texture" : string, (example "archer_idle", if the animations are defined the texture is unused)
 *              "size": glm::vec2, (the size of the image, if the collider is not defined, the collider will span the entire image)
 *              "collider" :
 *                  {
 *                      "size" : glm::vec2,  (default size will be the player size)
 *                      "offset": glm::vec2, (default {0, 0}),
 *                      "solid": bool (default true)
 *                  },
 *               "animations": (states must match the SpriteMovementState enum), (Animation size = size)
 *                {
 *                  "frameSpeed" : float, (default 0.2f)
 *                  "states" : [
 *                      {
 *                          "state" : "IDLE",
 *                          "frames:" x,
 *                          "rows" : x, (default to 1),
 *                          "type" : "SINGULAR" etc.. (default is continous)
 *                      }
 *                  ]
 *                },
 *                // do we need this?
 *                "custom":
 *                 {
 *                      // the above, is the basic information needed to create a game object
 *                      // to save/load specific state to a custom game object, such as Player, the player will serialize/deserialize
 *                      // from the custom section
 *                 }
 *          }
 *      ]
 * }
 */

using json = nlohmann::json;

namespace ObjectDefs
{
    static std::map<std::string, GameObjectDef> objectDefinitions;

    GameObjectDef& getObjectDefinition(const std::string& objectDef)
    {
        return objectDefinitions.at(objectDef);
    }

    // Split this function
    bool loadObjectDefinitions(const std::string& path)
    {
        std::fstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Object Definition Error! Cannot open the json file at " << path << "\n";
            return false;
        }

        json loaded = json::parse(file);
        if (!loaded.contains("definitions")) return false;

        // loop through object definition
        for (const auto& defJson : loaded["definitions"])
        {
            GameObjectDef def;

            // LOADING NAME - key for objectDefintion
            std::string name = defJson["name"];
            if (name.empty())
            {
                std::cout << "loadObjectDefinitions warning! no name, skipping definition\n";
                continue;
            } // no default for name skip

            // LOADING TEXTURE ID
            std::string textureId = defJson["textureId"];
            def.textureId = textureId.empty() ? "" : textureId; // can be empty, only if animations exits

            // LOADING SIZE
            glm::vec2 size = vec2FromJson(defJson["size"]);
            def.size = size;

            // LOADING COLLIDER
            if (defJson.contains("collider"))
            {
                json cJson = defJson["collider"];
                Collider collider;

                glm::vec2 cSize = vec2FromJson(cJson["size"]);
                collider.size = cSize;

                glm::vec2 cOffset = vec2FromJson(cJson["offset"]);
                collider.offset = cOffset;

                bool solid = cJson["solid"];
                collider.isSolid = solid;

                def.collider = collider;
            }

            // LOADING ANIMATIONS
            if (defJson.contains("animations"))
            {
                json aJson = defJson["animations"];
                std::unordered_map<SpriteMovementState, SpriteAnimation> animations;

                // LOADING THE FRAME SPEED FOR ALL ANIMATIONS
                float frameSpeed = defJson["frameSpeed"];

                for (const auto& animation : aJson["states"])
                {
                    SpriteAnimation spriteAnimation;

                    // LOADING ANIMATION STATE
                    std::string state = animation["state"];
                    SpriteMovementState movoementState = toSpriteMovementState(state);

                    // LOADING ANIMATION TEXTURE ID
                    std::string textureId = animation["textureId"];
                    spriteAnimation.textureID = textureId;

                    // LOADING FRAMES
                    int frames = animation["frames"];
                    spriteAnimation.columns = frames; // need to rename struct variable names

                    // LOADING ROWS
                    int rows = animation["rows"];
                    spriteAnimation.columns = rows;

                    // LOADING ANIMATION TYPE
                    if (animation.contains("type"))
                    {
                        std::string type = animation["type"];
                        AnimationType animType = toAnimationType(type);

                        spriteAnimation.animationType = animType;
                    }

                    // SETTING FRAME SIZE AND HEIGHT (equal to defined player size)
                    spriteAnimation.frameWidth = size.x;
                    spriteAnimation.frameHeight = size.y;

                    // SETTING FRAME DURATION (global)
                    spriteAnimation.frameDuration = frameSpeed; // inconsitent names

                    animations[movoementState] = spriteAnimation;
                }

                // SETTING ANIMATIONS
                def.animations = animations;
            }

            objectDefinitions[name] = def;
        }
        return true;
    }
}

// Is there a way to map directly whats in the struct to a string version
// What about restricting the use of getObjectDefintion or even thuse of game_object_def to games or gameObjects?
