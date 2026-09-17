#include "trexel.h"
#include "IRenderable.h"
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

    mPlayer = static_cast<MainScene*>(m_sceneManager.current())->getPlayer();
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
            if (mPlayer) mPlayer->move(action.direction);
            break;
        case GameActionType::Jump:
            if (mPlayer) mPlayer->jump();
            break;
    }
}
