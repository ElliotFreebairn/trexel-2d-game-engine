#include "enemy.h"
#include "collision.h"
#include "game_object.h"
#include "sprite.h"

#include <cstdlib>
#include <chrono>
#include <numbers>

Enemy::Enemy(const GameObjectDef& def, glm::vec2 startingPosition)
    : GameObject(def, startingPosition)
{

}

void Enemy::update(float dt)
{
    transform.position.x += (50 * dt) * m_facing.x;
    transform.flip = m_facing.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE;
    resolveSpriteState();
    if (visual.sprite) visual.sprite->update(dt);
}

void Enemy::evCollided(const CollisionInfo& hit)
{
    // walk away from whatever wall we hit. Setting (not toggling) means hitting
    // several stacked blocks in one frame still gives the same direction
    if (hit.normal.x != 0.0f)
    {
        m_facing.x = hit.normal.x;
    }
}

void Enemy::resolveSpriteState()
{
    if (!visual.sprite) return;

    if (m_isMoving) visual.sprite->play(SpriteMovementState::WALKING);
    else visual.sprite->play(SpriteMovementState::IDLE);
}
