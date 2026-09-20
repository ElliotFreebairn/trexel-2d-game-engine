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
    std::optional<glm::vec2> size;
    glm::vec2 offset = {0.0f, 0.0f};
};

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
