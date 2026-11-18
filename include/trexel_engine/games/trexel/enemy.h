#pragma once

#include "ISprite.h"
#include "collision.h"
#include "game_object.h"
#include "game_object_def.h"

class Enemy : public GameObject, public ISprite
{
public:
    Enemy(const GameObjectDef& def, glm::vec2 startingPosition);
    virtual ~Enemy() = default;

    // GameObject
    void update(float dt) override;
    void evCollided(const CollisionInfo& hit) override;

    // Sprite
    void resolveSpriteState() override;
private:
    glm::vec2 m_facing = glm::vec2(1.0f, 0.0f);

    bool m_isMoving = true;
};
