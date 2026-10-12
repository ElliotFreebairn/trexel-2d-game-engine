#pragma once

#include "common.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <bits/stdc++.h>

enum class Direction { UP, DOWN, LEFT, RIGHT };

enum class SpriteMovementState
{
    IDLE,
    WALKING,
    RUNNING,
    JUMPING,
    ATTACKING
};

enum class AnimationType
{
    SINGULAR,
    CONTINUOUS
};

struct SpriteAnimation
{
    std::string textureID;
    int frameWidth;
    int frameHeight;
    int columns;
    int rows = 1;
    float frameDuration = 0.15f;
    AnimationType animationType = AnimationType::CONTINUOUS;
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
    bool isFinished() const { return m_finished; }
    int currentFrame() const { return m_currentFrame; }

    // TEMP HELPER METHODS
    static AnimationType toAnimationType(std::string input);
    static SpriteMovementState toSpriteMovementState(std::string input);

private:
    std::unordered_map<SpriteMovementState, SpriteAnimation> m_animations;
    SpriteMovementState m_currentState = SpriteMovementState::IDLE;

    Direction m_direction = Direction::DOWN;

    int m_currentFrame = 0;
    float m_elapsed = 0.0f;
    bool m_finished = false;

    int currentRow(const SpriteAnimation& animation) const;
};
