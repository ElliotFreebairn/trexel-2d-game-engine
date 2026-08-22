#ifndef GAME_H
#define GAME_H

// Custom includes
#include "common.h"

// C++ includes
#include <vector>

struct Entity
{
    Position position;
    Size size;
    // Potential additons: sprite, text etc... especially for the renderer
};

/*
 * A game knows nothing about the renderer
 */
class Game
{
public:
    Game();
    ~Game();

    std::vector<Entity> get_entities() const { return entities; };
private:
    std::vector<Entity> entities;
};
#endif
