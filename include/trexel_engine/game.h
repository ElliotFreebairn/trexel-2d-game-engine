#pragma once

// Library includes
#include "IRenderable.h"

// Custom includes
#include "game_actions.h"
#include "scene_manager.h"

// C++ includes
#include <vector>


struct Entity
{
    glm::vec2 position;
    glm::vec2 size;
    // Potential additons: sprite, text etc... especially for the renderer
};

enum class DEFINED_OBJECT
{
    BLOCK,
    PLAYER
};

/*
 * A game knows nothing about the renderer
 */
class Game
{
public:
    virtual ~Game() = default;

    virtual void onInit() = 0;
    virtual void onUpdate(float deltaTime) { m_sceneManager.update(deltaTime); }
    virtual void onShutdown() = 0;
    virtual void handleAction(const GameAction& action) = 0;
    // game bundles up the visual state of the world into a dumb data structure
    virtual std::vector<const IRenderable*> getRenderables() { return m_sceneManager.getRenderables(); }

    bool isRunning() const { return m_isRunning; }

    // getters
    glm::vec2 getViewport() { return viewport; }
    // setters
    void setViewport(glm::vec2 viewport) { this->viewport = viewport; }

protected:
    SceneManager m_sceneManager;
    bool m_isRunning = true;

private:
    glm::vec2 viewport;
};
