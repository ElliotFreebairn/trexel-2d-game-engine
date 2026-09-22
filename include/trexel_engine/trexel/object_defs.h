#pragma once
#include "../collision.h"
#include "../game_object_def.h"
#include "../sprite.h"

namespace ObjectDefs
{
    inline GameObjectDef GrassBlock()
    {
        return GameObjectDef{
            .size = {50.0f, 50.0f},
            .textureId = "grass_block",
            .collider = Collider{ .isSolid = true, .size = {50.0f, 50.0f} }
        };
    }

    inline GameObjectDef Player()
    {
        const glm::vec2 frameSize = {128.0f, 128.0f};
        const glm::vec2 colliderSize =  {30.0f, 70.0f};
        const glm::vec2 offset = {50.0f, 55.0f};

        return GameObjectDef{
            .size = frameSize,
            .textureId = "player",
            .collider = Collider{
                .isSolid = true,
                .size = colliderSize,
                .offset = offset
            },
            .animations = {
                { SpriteMovementState::IDLE,    SpriteAnimation{"player_idle", 128, 128, 6, 1, 0.2f} },
                { SpriteMovementState::WALKING, SpriteAnimation{"player_walking", 128, 128, 8, 1, 0.2f} },
                { SpriteMovementState::JUMPING, SpriteAnimation{"player_jumping", 128, 128, 9, 1, 0.2f} },
            }
        };
    }
}
