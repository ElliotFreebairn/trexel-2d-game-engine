#pragma once

#include "ability.h"

class BowAbility : public IAbility
{
public:
    static constexpr float ARROW_SPEED = 900.0f; // pixels per second

    void trigger(Scene& scene, EntityId user, float facing) override;
};
