#include "trexel.h"
#include "bow_ability.h"
#include "enemy.h"
#include "sword_ability.h"
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
#include "bow_ability.h"

namespace
{
constexpr const char* OBJECT_DEFINITIONS_PATH = "/home/elliotf/Develop/Personal/trexel/data/definitions/objects.json";
}

Trexel::Trexel()
{

}

Trexel::~Trexel()
{
    m_sceneManager.current()->onExit();
    m_assetManager.saveManifest();

}


void Trexel::registerObjectTypes(ObjectFactory& factory)
{
    factory.registerType<Player>("Player");
    factory.registerType<SceneObject>("SceneObject");
    factory.registerType<Enemy>("Enemy");
    factory.registerType<Arrow>("Arrow");

    factory.registerAbility<BowAbility>("bow");
    factory.registerAbility<SwordAbility>("sword");
}

void Trexel::onUpdate(float deltaTime, const InputState& input)
{
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

void Trexel::intialiseAssets(AssetManager& assetManager)
{
    assetManager.loadManifest(MANIFEST_PATH);
}
