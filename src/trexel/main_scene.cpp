#include "main_scene.h"
#include "collision.h"
#include "scene.h"
#include "scene_object.h"

void MainScene::update(float dt)
{
    Scene::update(dt);
    if (m_player) resolveCollisions(*m_player);
}

void MainScene::onEnter()
{
    setBackground("background");

    auto& block = createObject<SceneObject>(
        glm::vec2(500.0f, 500.0f),
        glm::vec2(50.0f, 50.0f),
        "grass_block"
    );
    block.addCollider(Collider { .isSolid = true});

    m_player = &createObject<Player>(
        glm::vec2(450.0f, 400.0f),
        glm::vec2(128.0f, 128.0f),
        "player"
    );
    m_player->addCollider(Collider{
        .isSolid = true,
        .size = glm::vec2(50.0f, 70.0f),
        .offset = glm::vec2(40.0f, 60.0f)
    });
}
