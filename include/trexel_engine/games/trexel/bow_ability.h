#pragma once

#include "ability.h"

class BowAbility : public IAbility
{
public:
    static constexpr float ARROW_SPEED = 900.0f; // pixels per second

    void trigger(GameObject& user, glm::vec2 facing, Scene& scene);
};
