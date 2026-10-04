#include "main_scene.h"
#include "arrow.h"
#include "collision.h"
#include "object_defs.h"
#include "scene.h"
#include "scene_object.h"
#include "scene_serializer.h"

namespace
{
constexpr const char* WORLD_SAVE_PATH = "/home/elliotf/Develop/Personal/trexel/assets/saved_worlds/world.json";
}

MainScene::MainScene(glm::vec2 viewportSize, const GameObjectRegistry& registry)
    : Scene("main")
    , m_registry(registry)      // <- initializer list
{
}

Player* MainScene::getPlayer() const
{
    for (const auto& obj : m_objects)
    {
        if (Player* player = dynamic_cast<Player*>(obj.get()))
        {
            return player;
        }
    }
    return nullptr;
}

void MainScene::update(float dt)
{
    Scene::update(dt);

    if (Player* player = getPlayer())
    {
        resolveCollisions(*player);
        player->setGrounded(isGroundedOn(*player));
    }

    // Arrows react to a hit (stop/despawn) rather than resolve collisions, so don't use resolveCollisions
    for (auto& objPtr : m_objects)
    {
        Arrow* arrow = dynamic_cast<Arrow*>(objPtr.get());
        if (arrow && arrow->isActive() && overlapsSolid(*arrow, arrow->ownerId()))
        {
            arrow->evCollided(CollisionAxis::X);
        }
    }
}

void MainScene::buildEnvironment()
{
    createObject<SceneObject>(glm::vec2(500.0f, 600.0f), ObjectDefs::GrassBlock());
    createObject<SceneObject>(glm::vec2(550.0f, 600.0f), ObjectDefs::GrassBlock());
    createObject<SceneObject>(glm::vec2(600.0f, 600.0f), ObjectDefs::GrassBlock());
    createObject<SceneObject>(glm::vec2(650.0f, 600.0f), ObjectDefs::GrassBlock());
    createObject<SceneObject>(glm::vec2(700.0f, 600.0f), ObjectDefs::GrassBlock());
    createObject<SceneObject>(glm::vec2(750.0f, 600.0f), ObjectDefs::GrassBlock());
}

void MainScene::onEnter()
{
    setBackground("background");

    if (!SceneSerializer::load(*this, WORLD_SAVE_PATH, m_registry))
    {
        buildEnvironment();
        createObject<Player>(glm::vec2(450.0f, 400.0f));
    }
}

void MainScene::onExit()
{
    SceneSerializer::save(*this, WORLD_SAVE_PATH);
}

// Whaat can i spawn?
void MainScene::spawn(const std::string &prototype, glm::vec2 position)
{
    if (prototype == "block")
    {
        createObject<SceneObject>(position, ObjectDefs::GrassBlock());
    }
}
