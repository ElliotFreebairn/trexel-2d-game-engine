#ifndef TREXEL_H
#define TREXEL_H

// Custom includes
#include "IRenderable.h"
#include "game.h"
#include "game_actions.h"
#include "player.h"
#include "scene_object.h"
#include <memory>
#include <vector>

// C++ includes


/*
 * Temporaily house defined objects
 */


/*
 * Return a tReturnCode instead of void for critical functions
 */
class Trexel : public Game
{
public:
    Trexel();

    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onShutdown() override;

    void handleAction(const GameAction& action) override;
private:
    Player* mPlayer = nullptr;

    std::vector<std::unique_ptr<SceneObject>> sceneObjects;


    // HELPER FUNCTIONS (could be moved elsewhere)
    void spawnPlayer();
    void createScene();

};
#endif
