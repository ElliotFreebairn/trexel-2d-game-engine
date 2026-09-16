#pragma once

#include "../scene.h"
#include "player.h"

class MainScene : public Scene
{
public:
    MainScene() : Scene("main") {}

    void onEnter() override;

    Player* getPlayer() const { return mPlayer; }

private:
    Player* mPlayer = nullptr;
};
