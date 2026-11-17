#pragma once

#include "collision.h"
#include "game_object.h"
#include "game_object_def.h"

class Enemy : public GameObject
{
public:
    Enemy(const GameObjectDef& def, glm::vec2 startingPosition);
    virtual ~Enemy() = default;

    // GameObject
    void update(float dt) override;
    void evCollided(CollisionAxis axis) override;

private:

};
