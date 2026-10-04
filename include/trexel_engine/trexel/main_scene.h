// main_scene.h — changes needed
#pragma once
#include "game_object_registry.h"
#include "scene.h"
#include "player.h"

class MainScene : public Scene
{
public:
    MainScene(glm::vec2 viewport, const GameObjectRegistry& registry);

    void update(float dt) override;
    void onEnter() override;
    void onExit() override;
    void spawn(const std::string& prototype, glm::vec2 position) override;

    // No longer a stored field - see getPlayer() in main_scene.cpp.
    Player* getPlayer() const;
    glm::vec2 getViewport() const { return m_viewport; }

private:
    void buildEnvironment();

    glm::vec2 m_viewport;
    const GameObjectRegistry& m_registry;
};
