#ifndef GAME_H
#define GAME_H

// C++ includes
#include <vector>

/*
 * For now!
 *  Define an entity struct
 */

struct GPosition
{
    int x;
    int y;
};

struct GSize
{
    int width;
    int height;
};

struct Entity
{
    GPosition position;
    GSize size;

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
