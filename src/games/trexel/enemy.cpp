#include "enemy.h"
#include "collision.h"
#include "common.h"
#include "game_object.h"
#include "sprite.h"

#include <cstdlib>
#include <chrono>
#include <numbers>

Enemy::Enemy(const GameObjectDef& def, glm::vec2 startingPosition)
    : GameObject(def, startingPosition)
{

}

void Enemy::update(const FrameContext& ctx)
{
    body.velocity.x = 50.0f * m_facing.x;
    transform.flip = m_facing.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE;
}

void Enemy::evCollided(const CollisionInfo& hit)
{
    if (hit.normal.x != 0.0f)
    {
        m_facing.x = hit.normal.x; // trigger overlaps have normal 0
    }
}

SpriteMovementState Enemy::animationState()
{
    return SpriteMovementState::WALKING;
}
