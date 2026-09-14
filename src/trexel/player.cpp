#include "player.h"
#include "common.h"
#include "game_object.h"
#include <SDL_render.h>

Player::Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : GameObject(position, size, texture_id, colour)
{
    sprite = Sprite("walking_player", 128, 128, 8, 1, 0.2);
}

Player::Player() : GameObject()
{

}

void Player::update(float dt)
{
    if (sprite)
    {
        sprite->setMoving(isMoving);
    }
    updateSprite(dt);
    isMoving = false;
}

void Player::move(glm::vec2 delta)
{
    if (delta.x < 0.0f)
    {
        setFlip(Flip::HORIZONTAL);
    }
    else
    {
        setFlip(Flip::NONE);
    }

    position += delta * 200.0f;
    isMoving = true;
}
