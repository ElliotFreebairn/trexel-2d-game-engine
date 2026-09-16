#ifndef SPRITE_H
#define SPRITE_H

#include "common.h"

#include <iostream>
#include <unordered_map>

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

    void update(float deltaTime);
    void setDirection(Direction dir) { direction = dir; }

    // Single entry point
    void play(SpriteMovementState state);

    std::string getTextureID() const;
    Rect getSrcRect() const;

private:
    std::unordered_map<SpriteMovementState, SpriteAnimation> animations;
    SpriteMovementState currentState = SpriteMovementState::IDLE;

    Direction direction = Direction::DOWN;

    int currentFrame = 0;
    float elapsed = 0.0f;

    int currentRow(const SpriteAnimation& animation) const;
};
#endif
