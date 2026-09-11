#ifndef TREXEL_H
#define TREXEL_H

// Custom includes
#include "IRenderable.h"
#include "game.h"
#include "game_actions.h"
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
    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onShutdown() override;

    void handleAction(const GameAction& action) override;

    std::vector<const IRenderable*> getRenderables() const override;

private:
};
#endif
