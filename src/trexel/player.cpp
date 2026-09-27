#include "player.h"
#include "bow_item.h"
#include "collision.h"
#include "common.h"
#include "game_object.h"
#include "object_defs.h"
#include "scene.h"
#include "sprite.h"
#include <SDL_render.h>

// C++ includes
#include <iostream>

Player::Player(glm::vec2 position)
    : GameObject(position, ObjectDefs::Player())

{
    m_applyGravity = true;
}

Player::Player() : GameObject()
{

}

void Player::update(float dt)
{
    if (m_fireCooldown > 0.0f)
    {
        m_fireCooldown -= dt;
    }

    // The attack animation is one-shot (SINGULAR)
    if(m_isAttacking && m_sprite && m_sprite->isFinished())
    {
        m_isAttacking = false;
    }

    resolveSpriteState();
    updateSprite(dt);
    m_isMoving = false;
}

void Player::resolveSpriteState()
{
    if (!m_sprite) return;

    if (m_isAttacking) return;

    if (m_isJumping) m_sprite->play(SpriteMovementState::JUMPING);
    else if (m_isMoving) m_sprite->play(SpriteMovementState::WALKING);
    else m_sprite->play(SpriteMovementState::IDLE);
}

void Player::move(glm::vec2 delta)
{
    if (delta.x != 0.0f)
    {
        m_facing = glm::vec2(delta.x < 0.0f ? -1.0f : 1.0f, 0.0f);
    }
    setFlip(delta.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE);
    m_position += delta * 200.0f;
    m_isMoving = true;
}

void Player::jump()
{
    if (m_isJumping) return;
    m_isJumping = true;
    m_velocityY = JUMP_FORCE;
}

void Player::fire()
{
    if (m_fireCooldown > 0.0f) return;

    m_isAttacking = true;
    m_hasFiredThisAttack = false;
    m_fireCooldown = BOW_COOLDOWN;

    if (m_sprite) m_sprite->play(SpriteMovementState::ATTACKING);
}

bool Player::wantsToReleaseArrow() const
{
    return m_isAttacking
        &&  !m_hasFiredThisAttack
        && m_sprite
        && m_sprite->currentFrame() >= RELEASE_FRAME;
}

void Player::releaseArrow(Scene& scene)
{
    m_bow.onUse(*this, scene);
    m_hasFiredThisAttack = true;
}

void Player::evCollided(CollisionAxis axis)
{
    if(m_isJumping && axis == CollisionAxis::Y)
    {
        m_isJumping = false;
        m_velocityY = 0.0f;
    }
}
