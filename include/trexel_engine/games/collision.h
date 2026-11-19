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

class GameObject;

/*
 * Passed to GameObject::evCollided when a collision is resolved
 *
 * normal points away from the surface that was hit, i.e. the direction the mover was pushed:
 *      (-1, 0) wall on the right   (1, 0) wall on the left
 *      (0, -1) standing on floor   (0, 1) hit a ceiling
 */
struct CollisionInfo
{
    glm::vec2 normal {0.0f};
    GameObject* other = nullptr;
};

inline bool aabbIntersects(const AABB& a, const AABB& b)
{
    constexpr float EPS = 0.001f;
    return a.min.x < b.max.x - EPS && a.max.x > b.min.x + EPS
        && a.min.y < b.max.y - EPS && a.max.y > b.min.y + EPS;
}
