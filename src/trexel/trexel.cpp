#include "trexel.h"
#include "IRenderable.h"
#include "asset.h"
#include "asset_manager.h"
#include "game_actions.h"
#include "game_object.h"
#include "game_object_def.h"
#include "main_scene.h"
#include "player.h"
#include "scene_object.h"
#include <memory>
#include <vector>

Trexel::Trexel()
{

}

Trexel::~Trexel()
{
    onShutdown();
}


void Trexel::registerObjectTypes(GameObjectRegistry &registry)
{
    registry.registerType("Player", [](glm::vec2 position, const GameObjectDef&) {
        return std::make_unique<Player>(position);
    });

    registry.registerType("SceneObject", [](glm::vec2 position, const GameObjectDef& def) {
        return std::make_unique<SceneObject>(position, def);
    });
}

Player* Trexel::getPlayer() const
{
    MainScene* scene = dynamic_cast<MainScene*>(m_sceneManager.current());
    return scene ? scene->getPlayer() : nullptr;
}

void Trexel::onUpdate(float deltaTime, const InputState& input)
{
    if (Scene* scene = m_sceneManager.current())
    {
        scene->snapshotPositions();
    }

    if (Player* player = getPlayer())
    {
        if (input.move != glm::vec2(0.0f))
        {
            player->move(input.move * deltaTime);
        }
        if (input.jumpPressed)
        {
            player->jump();
        }
        if (input.firePressed)
        {
           player->fire();
        }

        if (player->wantsToReleaseArrow())
        {
            if (MainScene* scene = dynamic_cast<MainScene*>(m_sceneManager.current()))
            {
                player->releaseArrow(*scene);
            }
        }
    }

    Game::onUpdate(deltaTime, input);
}

void Trexel::onInit()
{
    m_sceneManager.registerScene("main", [this] { return std::make_unique<MainScene>(this->getViewport(), this->getObjectRegistry()); });
    m_sceneManager.switchTo("main");
}

void Trexel::onShutdown()
{
    m_sceneManager.current()->onExit();
}

void Trexel::intialiseAssets(AssetManager &assets)
{
    // Background / Environment
    assets.addAsset("background", "assets/trexel/medieval/background/background.png", AssetType::Texture);
    assets.addAsset("grass_block", "assets/trexel/medieval/platformer/grass_block.png", AssetType::Texture);

    // player
    assets.addAsset("player_idle", "assets/trexel/player/idle-sheet.png", AssetType::Texture);
    assets.addAsset("player_walking", "assets/trexel/player/walk-sheet.png", AssetType::Texture);
    assets.addAsset("player_jumping", "assets/trexel/player/jump-sheet.png", AssetType::Texture);
    assets.addAsset("player_shooting", "assets/trexel/player/shot.png", AssetType::Texture);

    // bow
    assets.addAsset("arrow", "assets/trexel/player/arrow.png", AssetType::Texture);
}
