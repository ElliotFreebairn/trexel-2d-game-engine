#ifndef GAME_H
#define GAME_H


// Library includes
#include "IRenderable.h"
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
    virtual ~Game() = default;

    virtual void onInit() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onShutdown() = 0;

    // game bundles up the visual state of the world into a dumb data structure
    virtual std::vector<const IRenderable*> getRenderables() const = 0;

    bool isRunning() const { return mIsRunning; }

protected:
    bool mIsRunning = true;
};
#endif
