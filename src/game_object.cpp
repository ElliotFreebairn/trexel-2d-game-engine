#include "game_object.h"
#include "collision.h"
#include "common.h"
#include "game_object_def.h"
#include "property.h"
#include <optional>

#include <SDL_image.h>
#include <variant>

GameObject::GameObject(glm::vec2 position, const GameObjectDef& def)
    : m_position(position), m_prevPosition(position), m_size(def.size), m_textureId(def.textureId), m_colour(def.colour)
{
    if (def.collider) addCollider(*def.collider);

    if (!def.animations.empty())
    {
        m_sprite = Sprite();
        for (auto& [state, animation] : def.animations)
        {
            m_sprite->addAnimation(state, animation);
        }
    }
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
        return AABB{position, position + m_size};
    }

    const Collider& c = *m_collider;

    float offsetX = c.offset.x;
    if (m_flip == Flip::HORIZONTAL)
    {
        offsetX = m_size.x - c.offset.x - c.size.x;
    }

    glm::vec2 min = position + glm::vec2(offsetX, c.offset.y);
    glm::vec2 max = min + c.size;
    return AABB{min, max};
}

AABB GameObject::getBounds() const
{
    return getBoundsAt(m_position);
}


// Editor
std::vector<Property> GameObject::properties() const
{
    return {
        Property("position", m_position),
        Property("size", m_size),
        Property("angle", static_cast<float>(m_angle)),
        Property("visible", m_isVisible),
        Property("active", m_isActive, /*READ ONLY */true),
    };
}

void GameObject::setProperty(std::string name, const PropertyValue& value)
{
    if (name == "position")
    {
        m_position = std::get<glm::vec2>(value);
        m_prevPosition = m_position;
    }
    else if (name == "size") m_size = std::get<glm::vec2>(value);
    else if (name == "angle") m_angle = std::get<float>(value);
    else if (name == "visible") m_isVisible = std::get<bool>(value);
}
