#include "game_object.h"
#include "common.h"
#include <optional>

#include <SDL_image.h>

GameObject::GameObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
{
    this->position = position;
    this->size = size;
    this->textureID = texture_id;
    this->colour = colour;
    // Better of stopping initalisation via constructor() :
}

GameObject::GameObject()
{
    this->position = {0.0f, 0.0f};
    this->size = {0.0f, 0.0f};
}

GameObject::~GameObject()
{

}

std::string GameObject::getTextureID() const
{
    return sprite ? sprite->getTextureId() : textureID;
}

std::optional<Rect> GameObject::getSourceRect() const
{
    return sprite ? std::optional(sprite->getSrcRect()) : std::nullopt;
}
