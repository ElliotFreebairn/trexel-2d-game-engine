#include "game_object.h"
#include "collision.h"
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

AABB GameObject::getBounds() const
{
    glm::vec2 origin = m_position;
    glm::vec2 size = m_size;

    if (hasCollider())
    {
        const Collider& c = *m_collider;

        origin += c.offset;
        size = c.size.value_or(m_size);
    }

    AABB aabb;
    aabb.min = origin;
    aabb.max = origin + size;
    return aabb;
}
