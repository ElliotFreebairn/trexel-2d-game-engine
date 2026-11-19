#include "trexel.h"
#include "asset_manager.h"
#include "bow_ability.h"
#include "main_scene.h"
#include "sword_ability.h"

#include <memory>

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
    // No classes to register: definitions describe entities by their components
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
