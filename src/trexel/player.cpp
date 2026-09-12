#include "player.h"
#include "game_object.h"

Player::Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : GameObject(position, size, texture_id, colour)
{

}

Player::Player() : GameObject()
{

}

void Player::update()
{

}

void Player::move(glm::vec2 delta)
{
    position += delta * 200.0f;
}
