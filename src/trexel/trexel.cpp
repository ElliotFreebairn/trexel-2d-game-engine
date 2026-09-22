#include "trexel.h"
#include "IRenderable.h"
#include "asset.h"
#include "asset_manager.h"
#include "game_actions.h"
#include "game_object.h"
#include "main_scene.h"
#include "player.h"
#include "scene_object.h"
#include <memory>
#include <vector>

Trexel::Trexel()
{
    onInit();
}

Trexel::~Trexel()
{
}

void Trexel::onInit()
{
    m_sceneManager.registerScene("main", [this] { return std::make_unique<MainScene>(this->getViewport());});
    m_sceneManager.switchTo("main");
    m_player = static_cast<MainScene*>(m_sceneManager.current())->getPlayer();
}

void Trexel::onShutdown()
{

}

// Change to event
void Trexel::handleAction(const GameAction& action)
{
    switch(action.type)
    {
        case GameActionType::SpawnPlayer:
            break;
        case GameActionType::SpawnBlock:
            break;
        case GameActionType::MovePlayer:
            if (m_player) m_player->move(action.direction);
            break;
        case GameActionType::Jump:
            if (m_player) m_player->jump();
            break;
    }
}

void Trexel::intialiseAssets(AssetManager &assets)
{
    assets.addAsset("background", "assets/trexel/world/background.png", AssetType::Texture);
    assets.addAsset("grass_block", "assets/trexel/world/dirt_block.png", AssetType::Texture);

    // player
    assets.addAsset("player_idle", "assets/trexel/player/idle-sheet.png", AssetType::Texture);
    assets.addAsset("player_walking", "assets/trexel/player/walk-sheet.png", AssetType::Texture);
    assets.addAsset("player_jumping", "assets/trexel/player/jump-sheet.png", AssetType::Texture);
}
