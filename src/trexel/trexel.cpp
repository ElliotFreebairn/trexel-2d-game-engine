#include "trexel.h"
#include "IRenderable.h"
#include "game_actions.h"
#include "game_object.h"
#include "player.h"
#include "scene_object.h"
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
    createScene();
    spawnPlayer();
}

void Trexel::onUpdate(float deltaTime)
{
    for (auto& object : objects)
    {

    }

    mPlayer->update(deltaTime);
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

/*
 * HELPER FUNCTIONS
 */
void Trexel::spawnPlayer()
{
    std::unique_ptr<Player> player;

    player = std::make_unique<Player> (
        glm::vec2(50.0f, 50.0f),
        glm::vec2(128.0f, 128.0f),
        "player"
    );

    mPlayer = player.get();
    addObject(std::move(player));
}

void Trexel::createScene()
{
    std::unique_ptr<SceneObject> sceneObject1;

    sceneObject1 = std::make_unique<SceneObject> (
        glm::vec2(500.0f, 500.0f),
        glm::vec2(50.0f, 50.0f),
        "grass_block"
    );

    addObject(std::move(sceneObject1));
}
