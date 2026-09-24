#pragma once

#include "entity.h"

class Game;
class CommandQueue;

struct EditorState
{
    bool playing = false;
    bool showColliders = false;
    EntityId selected = INVALID_ENTITY;
};

struct EditorContext
{
    const Game& game;
    CommandQueue& commands;
    EditorState& state;
};
