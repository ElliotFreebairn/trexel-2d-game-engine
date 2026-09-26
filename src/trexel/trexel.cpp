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
}

Trexel::~Trexel()
{
}

Player* Trexel::getPlayer() const
{
    MainScene* scene = dynamic_cast<MainScene*>(m_sceneManager.current());
    return scene ? scene->getPlayer() : nullptr;
}

void Trexel::onUpdate(float deltaTime, const InputState& input)
{
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
            if (MainScene* scene = dynamic_cast<MainScene*>(m_sceneManager.current()))
            {
                std::cout << "find dynamic main\n";
                player->fire(*scene);
            }
        }
    }

    Game::onUpdate(deltaTime, input);
}

void Trexel::onInit()
{
    m_sceneManager.registerScene("main", [this] { return std::make_unique<MainScene>(this->getViewport());});
    m_sceneManager.switchTo("main");
}

void Trexel::onShutdown()
{

}

void Trexel::intialiseAssets(AssetManager &assets)
{
    assets.addAsset("background", "assets/trexel/world/background-2.png", AssetType::Texture);
    assets.addAsset("grass_block", "assets/trexel/world/dirt_block.png", AssetType::Texture);

    // player
    assets.addAsset("player_idle", "assets/trexel/player/idle-sheet.png", AssetType::Texture);
    assets.addAsset("player_walking", "assets/trexel/player/walk-sheet.png", AssetType::Texture);
    assets.addAsset("player_jumping", "assets/trexel/player/jump-sheet.png", AssetType::Texture);

    // bow
    assets.addAsset("arrow", "assets/trexel/player/arrow.png", AssetType::Texture);
}
