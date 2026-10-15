#pragma once
#include <glm/glm.hpp>
#include <string>
#include <optional>
#include <unordered_map>
#include "collision.h"
#include "sprite.h"

/*
 * Everything needed to build a GameObject, loaded from assets/definitions/objects.json
 * className picks which registered C++ class (Player, SceneObject...) gets built
 */
struct GameObjectDef
{
    std::string name;
    std::string className = "SceneObject";

    glm::vec2 size {0.0f};
    std::string textureId;
    std::optional<glm::vec3> colour;
    std::optional<Collider> collider;
    std::unordered_map<SpriteMovementState, SpriteAnimation> animations;

    bool gravity = false;
};
