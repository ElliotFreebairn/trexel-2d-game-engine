#pragma once

#include <glm/glm.hpp>

enum class GameActionType
{
    SpawnPlayer,
    SpawnBlock,
    MovePlayer,
    Jump
};

struct GameAction
{
    GameActionType type;
    glm::vec2 position;
    glm::vec2 direction;
};
