#pragma once

#include "IUsable.h"
#include "item.h"

constexpr float BOW_COOLDOWN = 0.4f;
constexpr float ARROW_SPEED = 900.0f; // pixels per second


class BowItem : public Item, public IUsable
{
public:
    BowItem() : Item("Bow", "Fires an arrow") {}

    // IUsable
    void onUse(Player& player, Scene& scene);
};
