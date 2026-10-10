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
// implement a generic method for this, get rid of this later!
SpriteMovementState toSpriteMovementState(std::string input)
{
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    if (input == "IDLE") return SpriteMovementState::IDLE;
    if (input == "WALKING") return SpriteMovementState::WALKING;
    if (input == "RUNNING") return SpriteMovementState::RUNNING;
    if (input == "JUMPING") return SpriteMovementState::JUMPING;
    if (input == "ATTACKING") return  SpriteMovementState::ATTACKING;

    // by default
    return SpriteMovementState::IDLE;
}

enum class AnimationType
{
    SINGULAR,
    CONTINUOUS
};
AnimationType toAnimationType(std::string input)
{
   std::transform(input.begin(), input.end(), input.begin(), ::toupper);
   if (input == "SINGULAR") return AnimationType::SINGULAR;
   if (input == "CONTINOUS") return AnimationType::CONTINUOUS;

   return AnimationType::CONTINUOUS;
}

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

private:
    std::unordered_map<SpriteMovementState, SpriteAnimation> m_animations;
    SpriteMovementState m_currentState = SpriteMovementState::IDLE;

    Direction m_direction = Direction::DOWN;

    int m_currentFrame = 0;
    float m_elapsed = 0.0f;
    bool m_finished = false;

    int currentRow(const SpriteAnimation& animation) const;
};
