#pragma once

#include "ability.h"

class SwordAbility : public IAbility
{
public:
    static constexpr float REACH = 50.0f; // pixels past the edge of the user's collider (matches the swing in knight/Attack_1.png)
    static constexpr int   DAMAGE = 1;
    void trigger(Scene& scene, EntityId user, float facing) override;
};
