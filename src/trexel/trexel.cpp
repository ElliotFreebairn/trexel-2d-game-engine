#include "trexel.h"
#include "IRenderable.h"
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
    m_sceneManager.registerScene("main", [] { return std::make_unique<MainScene>(); });
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
    assets.addAsset("player", "assets/player.png", AssetType::Texture);
    assets.addAsset("grass_block", "assets/tblock.png", AssetType::Texture);
}
