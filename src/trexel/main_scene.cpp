#include "main_scene.h"
#include "arrow.h"
#include "collision.h"
#include "object_defs.h"
#include "scene.h"
#include "scene_object.h"
#include <cmath>

MainScene::~MainScene()
{
    onExit();
}

void MainScene::update(float dt)
{
    Scene::update(dt);

    if (m_player)
    {
        resolveCollisions(*m_player);
        m_player->setGrounded(isGroundedOn(*m_player));
    }

    // Arrows react to a hit (stop/despawn) rather than resolve collsions, so don't use resolveCollisions
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

    // temp helper function to build map
    //buildEnvironment();
    //m_player = &createObject<Player>(glm::vec2(450.0f, 400.0f));

    loadScene("/home/elliotf/Develop/Personal/trexel/assets/saved_worlds/world.json");
}

void MainScene::onExit()
{
    saveScene("/home/elliotf/Develop/Personal/trexel/assets/saved_worlds/world.json");
}

// Whaat can i spawn?
void MainScene::spawn(const std::string &prototype, glm::vec2 position)
{
    if (prototype == "block")
    {
        createObject<SceneObject>(position, ObjectDefs::GrassBlock());
    }
}
