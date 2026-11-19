#include "main_scene.h"
#include "ecs/components.h"
#include "input_state.h"
#include "scene.h"
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

    // Definitions can mark several entities as player controlled, keep only the oldest.
    // Lowest id (not "first found") because unordered_map order isn't predictable
    EntityId first = INVALID_ENTITY;
    registry().each<PlayerControlled>([&](EntityId e, PlayerControlled&) {
        if (first == INVALID_ENTITY || e < first) first = e;
    });
    if (first != INVALID_ENTITY) setControlled(first);
}

void MainScene::onExit()
{
    SceneSerializer::save(*this, WORLD_SAVE_PATH);
}
