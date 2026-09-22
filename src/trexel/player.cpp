#include "player.h"
#include "collision.h"
#include "common.h"
#include "game_object.h"
#include "sprite.h"
#include <SDL_render.h>

// C++ includes
#include <iostream>

Player::Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : GameObject(position, size, texture_id, colour)
{
    m_sprite = Sprite();
    setupSprite();
}

Player::Player() : GameObject()
{

}

void Player::update(float dt)
{
    if (m_isJumping)
    {
        m_position.y += m_velocityY * dt;

        m_velocityY += GRAVITY * dt;

        if (m_position.y >= m_lastJumpPoint)
        {
            m_position.y = m_lastJumpPoint;
            m_isJumping = false;
            m_velocityY = 0.0f;
        }
    }

    resolveSpriteState();
    updateSprite(dt);
    m_isMoving = false;
}

void Player::resolveSpriteState()
{
    if (!m_sprite) return;

    if (m_isJumping) m_sprite->play(SpriteMovementState::JUMPING);
    else if (m_isMoving) m_sprite->play(SpriteMovementState::WALKING);
    else m_sprite->play(SpriteMovementState::IDLE);
}

void Player::move(glm::vec2 delta)
{
    setFlip(delta.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE);
    m_position += delta * 200.0f;
    m_isMoving = true;
}

void Player::jump()
{
    if (m_isJumping) return;
    m_isJumping = true;
    m_velocityY = JUMP_FORCE;
    m_lastJumpPoint = m_position.y;
}

void Player::evCollided(CollisionAxis axis)
{
    if(m_isJumping && axis == CollisionAxis::Y)
    {
        m_isJumping = false;
        m_velocityY = 0.0f;
    }
}

void Player::setupSprite()
{
    if (!m_sprite) return;

    m_sprite->addAnimation(SpriteMovementState::IDLE, SpriteAnimation{
        "player_idle", 128, 128, 6, 1, 0.2f
    });

    m_sprite->addAnimation(SpriteMovementState::WALKING, SpriteAnimation{
        "player_walking", 128, 128, 8, 1, 0.2f
    });

    m_sprite->addAnimation(SpriteMovementState::JUMPING, SpriteAnimation{
        "player_jumping", 128, 128, 9, 1, 0.2f
    });
}
