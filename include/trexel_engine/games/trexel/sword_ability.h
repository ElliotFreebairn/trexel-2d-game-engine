#pragma once

#include "ability.h"

/*
 * Melee swing: checks a box directly in front of the user's collider on the trigger frame
 * and reports everything inside it.
 */
class SwordAbility : public IAbility
{
public:
    static constexpr float REACH = 50.0f; // pixels past the edge of the user's collider (matches the swing in knight/Attack_1.png)

    void trigger(GameObject& user, glm::vec2 facing, Scene& scene) override;
};
