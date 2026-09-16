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
    ~Trexel();

    void onInit() override;
    void onShutdown() override;

    void handleAction(const GameAction& action) override;
private:
    Player* mPlayer = nullptr;
};
#endif
