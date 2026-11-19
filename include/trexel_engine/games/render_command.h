#pragma once

#include "collision.h"
#include "common.h"

#include <glm/glm.hpp>
#include <optional>
#include <string>

/*
 * Everything the renderer needs to draw one thing. Built fresh each frame by Scene
 * Plain data: the renderer gets a copy, so it can't change game state
 */
struct RenderCommand
{
    glm::vec2 position {0.0f};
    glm::vec2 size     {0.0f};
    std::string textureId;
    std::optional<Rect> sourceRect;      // nullopt = whole texture
    std::optional<glm::vec3> colour;     // fallback if the texture is missing
    double angle = 0.0;
    Flip flip = Flip::NONE;
    std::optional<AABB> collider;        // debug drawing
};
