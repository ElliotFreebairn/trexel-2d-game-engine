#include "sprite.h"

#include "iostream"

Sprite::Sprite(std::string texId, int fw, int fh, int cols, int rows, float dur)
: textureId(texId), frameWidth(fw), frameHeight(fh), columns(cols), rows(rows), frameDuration(dur) {}

void Sprite::update(float deltaTime)
{
    if (!moving)
    {
        currentFrame = 0;
        elapsed = 0.0f;
        return;
    }

    elapsed += deltaTime;
    if (elapsed >= frameDuration)
    {
        elapsed -= frameDuration;
        currentFrame = (currentFrame + 1) % (columns * rows);
    }
}

Rect Sprite::getSrcRect() const
{
    int col = currentFrame % columns;
    int row = currentFrame / columns;
    return Rect {col * frameWidth, row * frameHeight, frameWidth, frameHeight};
}
