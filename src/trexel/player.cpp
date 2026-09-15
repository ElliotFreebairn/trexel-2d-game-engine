#include "player.h"
#include "common.h"
#include "game_object.h"
#include "sprite.h"
#include <SDL_render.h>

Player::Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : GameObject(position, size, texture_id, colour)
{
    sprite = Sprite();
    setupSprite();
}

Player::Player() : GameObject()
{

}

void Player::update(float dt)
{
    if (isJumping)
    {
        jumpElapsed += dt;
        if (jumpElapsed >= jumpDuration)
        {
            isJumping = false;
            jumpElapsed = 0.0f;
        }
    }

    resolveAnimationState();
    updateSprite(dt);
    isMoving = false;
}

void Player::resolveAnimationState()
{
    if (!sprite) return;

    if (isJumping) sprite->play(SpriteMovementState::JUMPING);
    else if (isMoving) sprite->play(SpriteMovementState::WALKING);
    else sprite->play(SpriteMovementState::IDLE);
}

void Player::move(glm::vec2 delta)
{
    setFlip(delta.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE);
    position += delta * 200.0f;
    isMoving = true;
}

void Player::jump()
{
    if (isJumping) return;
    isJumping = true;
    jumpElapsed = 0.0f;
}

void Player::setupSprite()
{
    if (!sprite) return;

    sprite->addAnimation(SpriteMovementState::IDLE, SpriteAnimation{
        "idle_player", 128, 128, 5, 1, 0.2f
    });

    sprite->addAnimation(SpriteMovementState::WALKING, SpriteAnimation{
        "walking_player", 128, 128, 8, 1, 0.2f
    });

    sprite->addAnimation(SpriteMovementState::JUMPING, SpriteAnimation{
        "jumping_player", 128, 128, 7, 1, 0.2f
    });
}
