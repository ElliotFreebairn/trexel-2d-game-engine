#pragma once

#include "collision.h"
#include "ecs/components.h"
#include "sprite.h"

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <unordered_map>

/*
 * Everything needed to spawn an entity, loaded from data/definitions/objects.json
 * Every optional field is a component: present means the entity gets it
 */
struct GameObjectDef
{
    std::string name;

    // Always present
    Transform transform;
    Visual visual;

    // Optional components
    std::optional<Collider>    collider;
    std::optional<Body>        body;
    std::optional<Movement>    movement;
    std::optional<AbilityDef>  attack;
    std::optional<Health>      health;
    std::optional<Projectile>  projectile;
    std::optional<DamageOnHit> damageOnHit;

    // Tags
    bool patrol = false;
    bool playerControlled = false; // "starts under player control"
};
