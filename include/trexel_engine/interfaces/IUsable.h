#pragma once

class Player;
class Scene;

class IUsable
{
public:
    virtual ~IUsable() = default;
    virtual void onUse(Player& player, Scene& scene) = 0;
};
