#include "sprite.h"

void Sprite::addAnimation(SpriteMovementState state, SpriteAnimation animation)
{
    animations[state] = std::move(animation);
}

void Sprite::play(SpriteMovementState state)
{
    if (state == currentState) return;
    if (animations.find(state) == animations.end()) return;
    currentState = state;
    currentFrame = 0;
    elapsed = 0.0f;
}

void Sprite::update(float deltaTime)
{
    auto it = animations.find(currentState);
    if (it == animations.end()) return;
    const SpriteAnimation& animation = it->second;

    elapsed += deltaTime;
    if (elapsed >= animation.frameDuration)
    {
        elapsed -= animation.frameDuration;
        currentFrame = (currentFrame + 1) % animation.columns;
    }
}

std::string Sprite::getTextureID() const
{
    auto it = animations.find(currentState);
    return it != animations.end() ? it->second.textureID : "";
}

int Sprite::currentRow(const SpriteAnimation &animation) const
{
    if (animation.rows <= 1) return 0;
    return static_cast<int>(direction) % animation.rows;
}

Rect Sprite::getSrcRect() const
{
    auto it = animations.find(currentState);
    if (it == animations.end()) return Rect{0, 0, 0, 0};

    const SpriteAnimation& animation = it->second;
    return Rect{
        currentFrame * animation.frameWidth,
        currentRow(animation) * animation.frameHeight,
        animation.frameWidth,
        animation.frameHeight
    };
}
