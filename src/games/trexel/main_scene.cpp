#include "main_scene.h"
#include "arrow.h"
#include "collision.h"
#include "game_object.h"
#include "input_state.h"
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

void MainScene::update(float dt, const InputState& input)
{
    Scene::update(dt, input);
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

    for (const auto& obj : m_objects)
    {
        if (dynamic_cast<Player*>(obj.get()))
        {
            setControlled(obj->entityId());
            break;
        }
    }
}

void MainScene::onExit()
{
    SceneSerializer::save(*this, WORLD_SAVE_PATH);
}
