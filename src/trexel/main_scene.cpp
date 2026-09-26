#include "main_scene.h"
#include "arrow.h"
#include "collision.h"
#include "object_defs.h"
#include "scene.h"
#include "scene_object.h"

void MainScene::update(float dt)
{
    Scene::update(dt);
    if (m_player) resolveCollisions(*m_player);

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

void MainScene::onEnter()
{
    setBackground("background");
    createObject<SceneObject>(glm::vec2(500.0f, 500.0f), ObjectDefs::GrassBlock());
    m_player = &createObject<Player>(glm::vec2(450.0f, 400.0f));
}

// Whaat can i spawn?
void MainScene::spawn(const std::string &prototype, glm::vec2 position)
{
    if (prototype == "block")
    {
        createObject<SceneObject>(position, ObjectDefs::GrassBlock());
    }
}
