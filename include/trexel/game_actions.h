#ifndef GAME_ACTION_H
#define GAME_ACTION_H

#include <glm/glm.hpp>

enum class GameActionType
{
    SpawnPlayer,
    SpawnBlock,
};

struct GameAction
{
    GameActionType type;
    glm::vec2 position;
};

#endif
