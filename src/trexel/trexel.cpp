#include "trexel.h"
#include "IRenderable.h"
#include "game_actions.h"
#include "game_object.h"
#include "player.h"
#include <memory>
#include <vector>

Trexel::Trexel()
{
    onInit();
}

void Trexel::onInit()
{
    /*
     * Create a player
     */
    spawnPlayer();
}

void Trexel::onUpdate(float deltaTime)
{
    for (auto& object : objects)
    {

    }
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
    }
}

/*
 * HELPER FUNCTIONS
 */
void Trexel::spawnPlayer()
{
    std::unique_ptr<Player> player;

    player = std::make_unique<Player> (
        glm::vec2(50.0f, 50.0f),
        glm::vec2(50.0f, 100.0f),
        "player"
    );

    mPlayer = player.get();
    addObject(std::move(player));
}
