#include "game_object.h"
#include "collision.h"
#include "common.h"
#include "game_object_def.h"
#include "json_utility.h"
#include "property.h"
#include <optional>

#include <variant>

namespace
{
constexpr float GRAVITY = 1500.0f;
constexpr float MAX_FALL_SPEED = 1000.0f;
}

GameObject::GameObject(const GameObjectDef& def, glm::vec2 position)
    : collider(def.collider), m_defName(def.name)
{
    transform.position = position;
    transform.prevPosition = position;
    transform.size = def.size;

    body.gravity = def.gravity;

    visual.textureId = def.textureId;
    visual.colour = def.colour;
    if (!def.animations.empty())
    {
        visual.sprite = Sprite();
        for (auto& [state, animation] : def.animations)
        {
            visual.sprite->addAnimation(state, animation);
        }
    }
}

void GameObject::applyGravity(float dt)
{
    if (!body.gravity) return;

    if (body.grounded && body.velocityY >= 0.0f)
    {
        body.velocityY = 0.0f;
        return;
    }

    body.velocityY += GRAVITY * dt;
    if (body.velocityY > MAX_FALL_SPEED) body.velocityY = MAX_FALL_SPEED;
    transform.position.y += body.velocityY * dt;
}

std::string GameObject::getTextureID() const
{
    return visual.sprite ? visual.sprite->getTextureID() : visual.textureId;
}

std::optional<Rect> GameObject::getSourceRect() const
{
    return visual.sprite ? std::optional(visual.sprite->getSrcRect()) : std::nullopt;
}

AABB GameObject::getBoundsAt(glm::vec2 position) const
{
    if (!collider)
    {
        return AABB{position, position + transform.size};
    }

    const Collider& c = *collider;

    float offsetX = c.offset.x;
    if (transform.flip == Flip::HORIZONTAL)
    {
        offsetX = transform.size.x - c.offset.x - c.size.x;
    }

    glm::vec2 min = position + glm::vec2(offsetX, c.offset.y);
    glm::vec2 max = min + c.size;
    return AABB{min, max};
}

AABB GameObject::getBounds() const
{
    return getBoundsAt(transform.position);
}


// Editor
std::vector<Property> GameObject::properties() const
{
    return {
        Property("def", m_defName, /*READ ONLY */true),
        Property("position", transform.position),
        Property("size", transform.size),
        Property("angle", static_cast<float>(transform.angle)),
        Property("visible", visual.visible),
        Property("active", m_isActive, /*READ ONLY */true),
    };
}

void GameObject::setProperty(std::string name, const PropertyValue& value)
{
    if (name == "position")
    {
        transform.position = std::get<glm::vec2>(value);
        transform.prevPosition = transform.position;
    }
    else if (name == "size") transform.size = std::get<glm::vec2>(value);
    else if (name == "angle") transform.angle = std::get<float>(value);
    else if (name == "visible") visual.visible = std::get<bool>(value);
}

// Serialization
nlohmann::json GameObject::serialize() const
{
    nlohmann::json j = nlohmann::json::object();
    j["def"] = m_defName;
    j["position"] = vec2ToJson(transform.position);
    j["size"] = vec2ToJson(transform.size);
    if (transform.angle != 0.0) j["angle"] = transform.angle;
    if (!visual.visible) j["visible"] = false;
    return j;
}

void GameObject::deSerialize(const nlohmann::json& j)
{
    if (j.contains("position"))
    {
        transform.position = vec2FromJson(j.at("position"));
        transform.prevPosition = transform.position;
    }
    if (j.contains("size")) transform.size = vec2FromJson(j.at("size"));
    transform.angle = j.value("angle", transform.angle);
    visual.visible = j.value("visible", visual.visible);
}
