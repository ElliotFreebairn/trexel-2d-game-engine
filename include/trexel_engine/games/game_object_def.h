#pragma once

#include "ability_registry.h"
#include "collision.h"
#include "ecs/components.h"
#include "sprite.h"

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <unordered_map>

// The attack a definition has, e.g. the archer's bow
struct AbilityDef
{
    std::string name;
    float cooldown = 0.4f;
    int triggerFrame = 0; // frame of the ATTACKING animation where the ability fires

    AbilityRegistry::Maker make; // resolved from name when loaded, never saved
};

/*
 * Everything needed to spawn an entity, loaded from data/definitions/objects.json
 * Every optional field is a component: present means the entity gets it
 */
struct GameObjectDef
{
    std::string name;

    // Always present: becomes Transform + Visual
    glm::vec2 size {0.0f};
    std::string textureId;
    std::optional<glm::vec3> colour;
    int layer = 0;
    std::unordered_map<SpriteMovementState, SpriteAnimation> animations;

    // Optional components
    std::optional<Collider>    collider;
    std::optional<Body>        body;
    std::optional<Locomotion>  locomotion;
    std::optional<AbilityDef>  attack;
    std::optional<Health>      health;
    std::optional<Projectile>  projectile;
    std::optional<DamageOnHit> damageOnHit;

    // Tags
    bool patrol = false;
    bool playerControlled = false; // "starts under player control"
};
