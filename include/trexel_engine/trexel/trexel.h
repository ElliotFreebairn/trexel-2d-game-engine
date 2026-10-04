#pragma once

// Custom includes
#include "asset_manager.h"
#include "game.h"
#include "game_actions.h"
#include "game_object_registry.h"
#include "input_state.h"
#include "player.h"

class Trexel : public Game
{
public:
    Trexel();
    ~Trexel();

    void onUpdate(float detlaTime, const InputState& state) override;
    void onShutdown() override;

protected:
    void onInit() override;
    void intialiseAssets(AssetManager& assets) override;
    void registerObjectTypes(GameObjectRegistry& registry) override;

private:
    Player* getPlayer() const;
};
