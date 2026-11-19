// main_scene.h — changes needed
#pragma once

#include "object_factory.h"
#include "scene.h"
#include "player.h"

class MainScene : public Scene
{
public:
    MainScene(glm::vec2 viewport, const ObjectFactory& factory);

    void update(float dt, const InputState& input) override;
    void onEnter() override;
    void onExit() override;

    // No longer a stored field - see getPlayer() in main_scene.cpp.
    glm::vec2 getViewport() const { return m_viewport; }

private:
    void buildEnvironment();

    glm::vec2 m_viewport;
};
