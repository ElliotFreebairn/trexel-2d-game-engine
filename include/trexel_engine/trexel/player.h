#ifndef PLAYER_H
#define PLAYER_H

// Custom includes
#include "game_object.h"
#include "sprite.h"

#include <optional>

// C++ includes
class Player : public GameObject
{
public:
    Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    Player();

    void move(glm::vec2 delta);
    void jump();
    void setupSprite();

    virtual ~Player() = default;

    void update(float dt) override;

private:
    bool isMoving = false;
    bool isJumping = false;
    float jumpDuration = 0.6f;
    float jumpElapsed = 0.0f;

    void resolveAnimationState();
};
#endif
