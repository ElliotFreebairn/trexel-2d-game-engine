#pragma once

// Custom includes
#include "ISprite.h"
#include "game_object.h"

#include <optional>

// C++ includes
class Player : public GameObject, public ISprite
{
public:
    Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    Player();
    virtual ~Player() = default;

    // Custom player functions
    void move(glm::vec2 delta);
    void jump();

    // Game object functions
    void update(float dt) override;
private:
    bool m_isMoving = false;
    bool m_isJumping = false;

    float m_velocityY = 0.0f;
    float m_lastJumpPoint = 0.0f;
    const float GRAVITY = 1500.0f;
    const float JUMP_FORCE = -600.0f;

    // Sprite functions
    void setupSprite() override;
    void resolveSpriteState() override;
};
