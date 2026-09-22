#pragma once
#include <glm/glm.hpp>
#include <optional>

struct AABB
{
    glm::vec2 min;
    glm::vec2 max;
};

struct Collider
{
    bool isSolid = true;
    glm::vec2 size {0.0f, 0.0f};
    glm::vec2 offset {0.0f, 0.0f};
};

enum class CollisionAxis { X, Y };

inline bool aabbOverlap(const AABB& a, const AABB& b)
{
    bool aIsToTheRightOfB = a.min.x > b.max.x;
    bool aIsToTheLeftOfB = a.max.x < b.min.x;
    bool aIsAboveB = a.max.y < b.min.y;
    bool aIsBelowB = a.min.y > b.max.y;

    return !(aIsToTheRightOfB
        || aIsToTheLeftOfB
        || aIsAboveB
        || aIsBelowB);
}
