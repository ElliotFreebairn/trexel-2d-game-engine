#include "main_scene.h"
#include "arrow.h"
#include "collision.h"
#include "game_object.h"
#include "player.h"
#include "scene.h"
#include "scene_object.h"
#include "scene_serializer.h"

namespace
{
constexpr const char* WORLD_SAVE_PATH = "/home/elliotf/Develop/Personal/trexel/data/saved_worlds/main_scene.json";
}

MainScene::MainScene(glm::vec2 viewportSize, const ObjectFactory& factory)
    : Scene("main", factory)
    , m_viewport(viewportSize)
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

    /*
     * Should this be moved to scene as it
     */
    for (const auto& obj : m_objects)
    {
        if (obj->collider.has_value() && obj->body.gravity == true)
        {
            resolveCollisions(*obj);
            obj->body.grounded = isGroundedOn(*obj);
        }
    }


    // Arrows react to a hit (stop/despawn) rather than resolve collisions, so don't use resolveCollisions
    for (auto& objPtr : m_objects)
    {
        Arrow* arrow = dynamic_cast<Arrow*>(objPtr.get());
        if (arrow && arrow->isActive() && overlapsSolid(*arrow, arrow->ownerId()))
        {
            arrow->evCollided({CollisionAxis::X});
        }
    }
}

void MainScene::buildEnvironment()
{
    for (size_t i = 0; i < 20; i++)
    {
        spawn("grass_block", glm::vec2(50.0f * i, 635.0f));
    }
}

void MainScene::onEnter()
{
    setBackground("background");

    if (!SceneSerializer::load(*this, WORLD_SAVE_PATH))
    {
        buildEnvironment();
        spawn("archer", glm::vec2(450.0f, 400.0f));
    }
}

void MainScene::onExit()
{
    SceneSerializer::save(*this, WORLD_SAVE_PATH);
}
