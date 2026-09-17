#include "game_object.h"
#include "common.h"
#include <optional>

#include <SDL_image.h>

GameObject::GameObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : m_position(position), m_size(size), m_textureId(texture_id), m_colour(colour)
{

}

GameObject::GameObject()
{
    m_position = {0.0f, 0.0f};
    m_size = {0.0f, 0.0f};
}

GameObject::~GameObject()
{

}

std::string GameObject::getTextureID() const
{
    return m_sprite ? m_sprite->getTextureID() : m_textureId;
}

std::optional<Rect> GameObject::getSourceRect() const
{
    return m_sprite ? std::optional(m_sprite->getSrcRect()) : std::nullopt;
}

AAB GameObject::getBounds() const
{
    glm::vec2 colliderSize = (m_collider && m_collider->size) ? *m_collider->size : m_size;
    glm::vec2 min = m_position + (m_collider ? m_collider->offset : glm::vec2{0.0f});
    return {min, min + colliderSize};
}
