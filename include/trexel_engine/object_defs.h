#pragma once
#include "collision.h"
#include "game_object_def.h"
#include "sprite.h"
#include "game.h"
#include "game_object.h"
#include "nlohmann/json.hpp"
#include <map>

namespace ObjectDefs
{
    bool loadObjectDefinitions(const std::string& path);
    GameObjectDef& getObjectDefinition(const std::string& objectDef);
}

/*
 * Object Definition JSON Example
 *
 * This is specific to how a game object looks, not its properties, that's why it is generic to the game object
 *
 * {
 *  "object_definitions":
 *      [
 *          {
 *              "def" : string, (example "ARCHER")
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
 *                  [
 *                      {
 *                          "state" : "IDLE",
 *                          "textureId", std::string
 *                          "frames:" x,
 *                          "rows" : x, (default to 1),
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






//    inline GameObjectDef GrassBlock()
//    {
//        return GameObjectDef{
//            .size = {50.0f, 50.0f},
//            .textureId = "grass_block",
//            .collider = Collider{ .isSolid = true, .size = {50.0f, 50.0f}},
//            .objectDefName = "GrassBlock"
//        };
//    }
//
//    inline GameObjectDef Player()
//    {
//        const glm::vec2 frameSize = {128.0f, 128.0f};
//        const glm::vec2 colliderSize =  {30.0f, 70.0f};
//        const glm::vec2 offset = {50.0f, 55.0f};
//
//        return GameObjectDef{
//            .size = frameSize,
//            .textureId = "player",
//            .collider = Collider{
//                .isSolid = true,
//                .size = colliderSize,
//                .offset = offset
//            },
//            .animations = {
//                { SpriteMovementState::IDLE,    SpriteAnimation{"player_idle", 128, 128, 6, 1, 0.2f} },
//                { SpriteMovementState::WALKING, SpriteAnimation{"player_walking", 128, 128, 8, 1, 0.2f} },
//                { SpriteMovementState::JUMPING, SpriteAnimation{"player_jumping", 128, 128, 9, 1, 0.2f} },
//                { SpriteMovementState::ATTACKING, SpriteAnimation{"player_shooting", 128, 128, 14, 1, 0.05f, AnimationType::SINGULAR}}
//            },
//            .objectDefName = "Player"
//        };
//    }
//
//    inline GameObjectDef Arrow()
//    {
//        // The arrow itself shoudn't block anything (only things it flies into), so solid should be false
//        return GameObjectDef
//        {
//            .size = {48.0f, 48.0f},
//            .textureId = "arrow",
//            .collider = Collider{ .isSolid = false, .size = {48.0f, 48.0f}},
//            .objectDefName = "Arrow"
//
//        };
//    };
//
//
//    inline GameObjectDef getGameObjectDef(const std::string& name)
//    {
//        static const std::unordered_map<std::string, GameObjectDef(*)()> defs = {
//            {"GrassBlock", &GrassBlock},
//            {"Player", &Player},
//            {"Arrow", &Arrow},
//        };
//
//        auto it = defs.find(name);
//        if (it == defs.end())
//        {
//            throw std::runtime_error("Unknown object def: " + name);
//        }
//        return it->second();
//    }
//}
//
