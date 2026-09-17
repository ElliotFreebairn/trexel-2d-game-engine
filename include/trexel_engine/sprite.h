#pragma once

#include "common.h"

#include <unordered_map>
#include <string>

enum class Direction { UP, DOWN, LEFT, RIGHT };

enum class SpriteMovementState
{
    IDLE,
    WALKING,
    RUNNING,
    JUMPING
};

struct SpriteAnimation
{
    std::string textureID;
    int frameWidth;
    int frameHeight;
    int columns;
    int rows = 1;
    float frameDuration = 0.15f;
};

class Sprite
{
public:
    Sprite() = default;

    void addAnimation(SpriteMovementState state, SpriteAnimation animation);

    void update(float dt);
    void setDirection(Direction dir) { m_direction = dir; }

    // Single entry point
    void play(SpriteMovementState state);

    std::string getTextureID() const;
    Rect getSrcRect() const;

private:
    std::unordered_map<SpriteMovementState, SpriteAnimation> m_animations;
    SpriteMovementState m_currentState = SpriteMovementState::IDLE;

    Direction m_direction = Direction::DOWN;

    int m_currentFrame = 0;
    float m_elapsed = 0.0f;

    int currentRow(const SpriteAnimation& animation) const;
};
