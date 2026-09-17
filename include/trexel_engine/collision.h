#pragma once
#include <glm/glm.hpp>
#include <optional>

struct AAB
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

inline bool aabOverlap(const AAB& a, const AAB& b)
{
    return a.min.x < b.max.x && a.max.x >= b.min.x &&
        a.min.y < b.max.y && a.max.y > b.min.y;
}
