#pragma once

#include "collision.h"
#include "game_object.h"
#include "game_object_def.h"
#include "sprite.h"

class Enemy : public GameObject
{
public:
    Enemy(const GameObjectDef& def, glm::vec2 startingPosition);
    virtual ~Enemy() = default;

    // GameObject
    void update(const FrameContext& ctx) override;
    void evCollided(const CollisionInfo& hit) override;

    // Sprite
    SpriteMovementState animationState() override;
private:
    glm::vec2 m_facing = glm::vec2(1.0f, 0.0f);
};
