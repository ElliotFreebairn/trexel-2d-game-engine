#pragma once

#include <glm/glm.hpp>

// Snapshot of player input
struct InputState
{
    glm::vec2 move = glm::vec2(0.0f);
    bool jumpHeld = false;
    bool jumpPressed = false;
};
