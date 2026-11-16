#pragma once
#include <glm/glm.hpp>
#include <string>
#include <optional>
#include <unordered_map>
#include "ability_registry.h"
#include "collision.h"
#include "sprite.h"

/*
 * An ability a character has, e.g. the archer's bow
 */
struct AbilityDef
{
  std::string name;
  float cooldown = 0.4f;
  int triggerFrame = 0; // frame of ATTACKING animation where the ability actually fires

  AbilityRegistry::Maker make; // resolved from name, never used
};

/*
 * Tuning values for anything controllerable that walks and jumps
 */
struct CharacterStats
{
    float moveSpeed = 200.0f;
    float jumpForce = -500.0f;
    std::optional<AbilityDef> ability;
};

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

    std::optional<CharacterStats> character; // only characters have this

    bool gravity = false;
};
