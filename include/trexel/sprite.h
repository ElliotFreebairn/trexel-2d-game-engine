#ifndef SPRITE_H
#define SPRITE_H

#include "common.h"

#include <iostream>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Sprite
{
public:
    Sprite(std::string textureId, int frameWidth, int frameHeight, int columns, int rows, float frameDuration = 1.0f);

    void update(float deltaTime);
    void setDirection(Direction dir) { direction = dir; }
    void setMoving(bool m) { moving = m; }
    Rect getSrcRect() const;
    std::string getTextureId() const { return textureId; }

    void setFrame(int frame) { currentFrame = frame % (columns * rows); }

private:
    Direction direction = Direction::DOWN;
    bool moving = false;

    std::string textureId;
    int frameWidth, frameHeight;
    int columns, rows;
    int horizontalOffset = 0;
    int currentFrame = 0;
    float frameDuration;
    float elapsed = 0.0f;
};
#endif
