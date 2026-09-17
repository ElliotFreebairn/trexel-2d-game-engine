#pragma once

#include "../scene.h"
#include "player.h"

class MainScene : public Scene
{
public:
    MainScene(glm::vec2 viewportSize) : Scene("main") { m_viewportSize = viewportSize; }

    void onEnter() override;

    // getters
    Player* getPlayer() const { return m_player; }
private:
    Player* m_player = nullptr;
    glm::vec2 m_viewportSize;
};
