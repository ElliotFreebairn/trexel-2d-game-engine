#include "game_object.h"
#include "collision.h"
#include "common.h"
#include <optional>

#include <SDL_image.h>

GameObject::GameObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : m_position(position), m_prevPosition(position), m_size(size), m_textureId(texture_id), m_colour(colour)
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

AABB GameObject::getBoundsAt(glm::vec2 position) const
{
    if (!m_collider)
    {
        return AABB{m_position, m_position + m_size};
    }

    const Collider& c = *m_collider;

    float offsetX = c.offset.x;
    if (m_flip == Flip::HORIZONTAL)
    {
        offsetX = m_size.x - c.offset.x - c.size.x;
    }

    glm::vec2 min = m_position + glm::vec2(offsetX, c.offset.y);
    glm::vec2 max = min + c.size;
    return AABB{min, max};
}

AABB GameObject::getBounds() const
{
    return getBoundsAt(m_position);
}
