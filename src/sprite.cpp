#include "sprite.h"

#include "iostream"

Sprite::Sprite(int fw, int fh, int cols, int rows, float dur)
: frameWidth(fw), frameHeight(fh), columns(cols), rows(rows), frameDuration(dur) {}

void Sprite::update(float deltaTime)
{
    elapsed += deltaTime;
    if (elapsed >= frameDuration)
    {
        elapsed -= frameDuration;
        currentFrame = (currentFrame + 1) % (columns * rows);
    }
}

SDL_Rect Sprite::getSrcRect() const
{
    int col = currentFrame % columns;
    int row = currentFrame / columns;
    return SDL_Rect {col * frameWidth, row * frameHeight, frameWidth, frameHeight};
}
