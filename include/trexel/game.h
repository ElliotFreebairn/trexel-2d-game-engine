#ifndef GAME_H
#define GAME_H


// Library includes
#include "glm/glm.hpp"

// Custom includes
#include "common.h"
#include "game_object.h"

// C++ includes
#include <memory>
#include <vector>

struct Entity
{
    glm::vec2 position;
    glm::vec2 size;
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

    void update();

    void spawn_block(glm::vec2 world_pos);

    const std::vector<std::unique_ptr<GameObject>>& get_entities() const { return game_objects; };
private:
    std::vector<std::unique_ptr<GameObject>> game_objects;
};
#endif
