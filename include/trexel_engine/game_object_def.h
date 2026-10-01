#pragma once
#include <glm/glm.hpp>
#include <string>
#include <optional>
#include <unordered_map>
#include "collision.h"
#include "sprite.h"

struct GameObjectDef
{
    glm::vec2 size;
    std::string textureId;
    std::optional<glm::vec3> colour;
    std::optional<Collider> collider;
    std::unordered_map<SpriteMovementState, SpriteAnimation> animations;
    std::string objectDefName;
};
