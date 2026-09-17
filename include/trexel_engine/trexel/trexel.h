#pragma once

// Custom includes
#include "asset_manager.h"
#include "game.h"
#include "game_actions.h"
#include "player.h"

class Trexel : public Game
{
public:
    Trexel();
    ~Trexel();

    void onInit() override;
    void onShutdown() override;
    void handleAction(const GameAction& action) override;
    void intialiseAssets(AssetManager& assets) override;

private:
    Player* m_player = nullptr;
};
