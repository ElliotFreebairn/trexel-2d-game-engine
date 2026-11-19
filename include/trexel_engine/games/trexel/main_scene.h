#pragma once

#include "object_factory.h"
#include "scene.h"

class MainScene : public Scene
{
public:
    MainScene(glm::vec2 viewport, const ObjectFactory& factory);

    void update(float dt, const InputState& input) override;
    void onEnter() override;
    void onExit() override;

    glm::vec2 getViewport() const { return m_viewport; }

private:
    void buildEnvironment();

    glm::vec2 m_viewport;
};
