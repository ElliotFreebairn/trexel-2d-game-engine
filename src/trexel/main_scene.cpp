#include "main_scene.h"
#include "scene_object.h"

void MainScene::onEnter()
{
    createObject<SceneObject>(
        glm::vec2(500.0f, 500.0f),
        glm::vec2(50.0f, 50.0f),
        "grass_block"
    );

    mPlayer = &createObject<Player>(
        glm::vec2(50.0f, 50.0f),
        glm::vec2(128.0f, 128.0f),
        "player"
    );
}
