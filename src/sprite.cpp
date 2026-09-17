#include "sprite.h"

void Sprite::addAnimation(SpriteMovementState state, SpriteAnimation animation)
{
    m_animations[state] = std::move(animation);
}

void Sprite::play(SpriteMovementState state)
{
    if (state == m_currentState) return;
    if (m_animations.find(state) == m_animations.end()) return;
    m_currentState = state;
    m_currentFrame = 0;
    m_elapsed = 0.0f;
}

void Sprite::update(float deltaTime)
{
    auto it = m_animations.find(m_currentState);
    if (it == m_animations.end()) return;
    const SpriteAnimation& animation = it->second;

    m_elapsed += deltaTime;
    if (m_elapsed >= animation.frameDuration)
    {
        m_elapsed -= animation.frameDuration;
        m_currentFrame = (m_currentFrame + 1) % animation.columns;
    }
}

std::string Sprite::getTextureID() const
{
    auto it = m_animations.find(m_currentState);
    return it != m_animations.end() ? it->second.textureID : "";
}

int Sprite::currentRow(const SpriteAnimation &animation) const
{
    if (animation.rows <= 1) return 0;
    return static_cast<int>(m_direction) % animation.rows;
}

Rect Sprite::getSrcRect() const
{
    auto it = m_animations.find(m_currentState);
    if (it == m_animations.end()) return Rect{0, 0, 0, 0};

    const SpriteAnimation& animation = it->second;
    return Rect{
        m_currentFrame * animation.frameWidth,
        currentRow(animation) * animation.frameHeight,
        animation.frameWidth,
        animation.frameHeight
    };
}
