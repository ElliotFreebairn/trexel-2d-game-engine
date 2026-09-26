#pragma once

// Custom includes
#include "ISprite.h"
#include "bow_item.h"
#include "collision.h"
#include "game_object.h"

#include <optional>

class Scene;

// C++ includes
class Player : public GameObject, public ISprite
{
public:
    Player(glm::vec2 position);
    Player();
    virtual ~Player() = default;

    void evCollided(CollisionAxis axis) override;

    // Custom player functions
    void move(glm::vec2 delta);
    void jump();
    void fire(Scene& scene);

    std::string typeName() const override { return "Player"; }
    glm::vec2 getFacing() const { return m_facing; }

    // Game object functions
    void update(float dt) override;
private:
    bool m_isMoving = false;
    bool m_isJumping = false;

    float m_velocityY = 0.0f;
    float m_lastJumpPoint = 0.0f;
    const float GRAVITY = 1500.0f;
    const float JUMP_FORCE = -600.0f;

    // bow
    glm::vec2 m_facing = glm::vec2(1.0f, 0.0f);
    float m_fireCooldown = 0.0f;
    BowItem m_bow;

    // Sprite functions
    void resolveSpriteState() override;
};
