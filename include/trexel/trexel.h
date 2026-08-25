#ifndef TREXEL_H
#define TREXEL_H

// Custom includes
#include "IRenderable.h"
#include "game.h"
#include "player.h"
#include <vector>

// C++ includes

/*
 * Return a tReturnCode instead of void for critical functions
 */
class Trexel : public Game
{
public:
    void onInit() override;
    void onUpdate(float deltaTime) override;
    void onShutdown() override;

    std::vector<const IRenderable*> getRenderables() const override;

    // temporary
    void spawnPlayer(glm::vec2 pos);
private:
    Player* player;
};
#endif
