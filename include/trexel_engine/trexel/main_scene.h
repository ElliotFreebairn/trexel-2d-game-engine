#pragma once

#include "../scene.h"
#include "player.h"

class MainScene : public Scene
{
public:
    MainScene() : Scene("main") {}

    void onEnter() override;

    // getters
    Player* getPlayer() const { return m_player; }
private:
    Player* m_player = nullptr;
};
