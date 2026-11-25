#pragma once
#include "entity.h"
#include <glm/glm.hpp>

struct AABB
{
    glm::vec2 min;
    glm::vec2 max;
};

// Used directly as a component
/*
 * Gets recorded by the physicsSystem the reaction systems read this afterwards
 *
 * normal points way from the surface self hit (-1, 0) = wall on the right etc...
 */
struct CollisionEvent
{
    EntityId self   = INVALID_ENTITY;
    EntityId other  = INVALID_ENTITY;
    glm::vec2 normal {0.0f};
};

inline bool aabbIntersects(const AABB& a, const AABB& b)
{
    constexpr float EPS = 0.001f;
    return a.min.x < b.max.x - EPS && a.max.x > b.min.x + EPS
        && a.min.y < b.max.y - EPS && a.max.y > b.min.y + EPS;
}
