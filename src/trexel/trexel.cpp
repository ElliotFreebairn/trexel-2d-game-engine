#include "trexel.h"
#include "IRenderable.h"
#include "asset.h"
#include "asset_manager.h"
#include "game_actions.h"
#include "game_object.h"
#include "game_object_def.h"
#include "arrow.h"
#include "main_scene.h"
#include "player.h"
#include "scene_object.h"
#include <memory>
#include <vector>

namespace
{
constexpr const char* OBJECT_DEFINITIONS_PATH = "/home/elliotf/Develop/Personal/trexel/assets/definitions/objects.json";
}

Trexel::Trexel()
{

}

Trexel::~Trexel()
{
    onShutdown();
}


void Trexel::registerObjectTypes(ObjectFactory& factory)
{
    factory.registerType<Player>("Player");
    factory.registerType<SceneObject>("SceneObject");
    factory.registerType<Arrow>("Arrow");
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
    // initialise objects allowed in the game
    registerObjectTypes(m_objectFactory);
    m_objectFactory.loadDefinitions(OBJECT_DEFINITIONS_PATH);

    m_sceneManager.registerScene("main", [this] { return std::make_unique<MainScene>(this->getViewport(), this->getObjectFactory()); });
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
