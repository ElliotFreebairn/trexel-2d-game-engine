#include "player.h"
#include "game_object.h"

Player::Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : GameObject(position, size, texture_id, colour)
{
    sprite = Sprite("player_sheet", 32, 32, 3, 1, 0.2);
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
    position += delta * 200.0f;
    isMoving = true;
}
