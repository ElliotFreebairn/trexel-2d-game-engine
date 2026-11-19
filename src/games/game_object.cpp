#include "game_object.h"
#include "collision.h"
#include "common.h"
#include "game_object_def.h"
#include "json_utility.h"
#include "property.h"
#include <optional>

#include <variant>

GameObject::GameObject(const GameObjectDef& def, glm::vec2 position)
    : collider(def.collider), m_def(&def)
{
    transform.position = position;
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

std::string GameObject::getTextureID() const
{
    return visual.sprite ? visual.sprite->getTextureID() : visual.textureId;
}

std::optional<Rect> GameObject::getSourceRect() const
{
    return visual.sprite ? std::optional(visual.sprite->getSrcRect()) : std::nullopt;
}

AABB GameObject::getBounds() const
{
    if (!collider)
    {
        return AABB{transform.position, transform.position + transform.size};
    }

    const Collider& c = *collider;

    float offsetX = c.offset.x;
    if (transform.flip == Flip::HORIZONTAL)
    {
        offsetX = transform.size.x - c.offset.x - c.size.x;
    }

    glm::vec2 min = transform.position + glm::vec2(offsetX, c.offset.y);
    glm::vec2 max = min + c.size;
    return AABB{min, max};
}


// Editor
std::vector<Property> GameObject::properties() const
{
    return {
        Property("def", m_def->name, /*READ ONLY */true),
        Property("position", transform.position),
        Property("size", transform.size),
        Property("angle", static_cast<float>(transform.angle)),
        Property("visible", visual.visible),
        Property("active", m_isActive, /*READ ONLY */true),
        Property("texture_id", visual.textureId)
    };
}

void GameObject::setProperty(std::string name, const PropertyValue& value)
{
    if (name == "position")
    {
        transform.position = std::get<glm::vec2>(value);
    }
    else if (name == "size") transform.size = std::get<glm::vec2>(value);
    else if (name == "angle") transform.angle = std::get<float>(value);
    else if (name == "visible") visual.visible = std::get<bool>(value);
    else if (name == "texture_id") visual.textureId = std::get<std::string>(value);
}

// Serialization
nlohmann::json GameObject::serialize() const
{
    nlohmann::json j = nlohmann::json::object();
    j["def"] = m_def->name;
    j["position"] = vec2ToJson(transform.position);
    j["size"] = vec2ToJson(transform.size);
    if (transform.angle != 0.0) j["angle"] = transform.angle;
    if (!visual.visible) j["visible"] = false;

    // optional, if definitions overode
    if (transform.size != m_def->size) j["size"] = vec2ToJson(transform.size);
    if (visual.textureId != m_def->textureId) j["textureId"] = visual.textureId;

    return j;
}

void GameObject::deSerialize(const nlohmann::json& j)
{
    if (j.contains("position"))
    {
        transform.position = vec2FromJson(j.at("position"));
    }
    if (j.contains("size")) transform.size = vec2FromJson(j.at("size"));
    transform.angle = j.value("angle", transform.angle);
    visual.visible = j.value("visible", visual.visible);
    visual.textureId = j.value("textureId", visual.textureId);
}
