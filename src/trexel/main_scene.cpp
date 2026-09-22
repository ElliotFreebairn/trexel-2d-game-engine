#include "main_scene.h"
#include "collision.h"
#include "object_defs.h"
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
    createObject<SceneObject>(glm::vec2(500.0f, 500.0f), ObjectDefs::GrassBlock());
    m_player = &createObject<Player>(glm::vec2(450.0f, 400.0f));
}
