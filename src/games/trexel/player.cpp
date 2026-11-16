#include "player.h"
#include "bow_item.h"
#include "collision.h"
#include "common.h"
#include "game_object.h"
#include "game_object_def.h"
#include "property.h"
#include "scene.h"
#include "sprite.h"
#include <SDL_render.h>

// C++ includes
#include <iostream>
#include <vector>

Player::Player(const GameObjectDef& def, glm::vec2 position)
    : GameObject(def, position)
{
    if (stats().ability && stats().ability->make)
    {
        m_ability = stats().ability->make();
    }
}

const CharacterStats& Player::stats() const
{
   // A "player" definition with no "character" still works, using deafults
   static const CharacterStats defaults;
   return def().character ? *def().character : defaults;
}

void Player::update(float dt)
{

    if (body.grounded && m_isJumping && body.velocityY >= 0.0f)
    {
        m_isJumping = false;
    }

    if (m_fireCooldown > 0.0f)
    {
        m_fireCooldown -= dt;
    }

    // The attack animation is one-shot (SINGULAR)
    if(m_isAttacking && visual.sprite && visual.sprite->isFinished())
    {
        m_isAttacking = false;
    }

    resolveSpriteState();
    if (visual.sprite) visual.sprite->update(dt);
    m_isMoving = false;
}

void Player::resolveSpriteState()
{
    if (!visual.sprite) return;
    if (m_isAttacking) return;

    if (m_isJumping) visual.sprite->play(SpriteMovementState::JUMPING);
    else if (m_isMoving) visual.sprite->play(SpriteMovementState::WALKING);
    else visual.sprite->play(SpriteMovementState::IDLE);
}

void Player::move(glm::vec2 delta)
{
    if (delta.x != 0.0f)
    {
        m_facing = glm::vec2(delta.x < 0.0f ? -1.0f : 1.0f, 0.0f);
        transform.flip = delta.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE;
    }
    transform.position += delta * stats().moveSpeed;
    m_isMoving = true;
}

void Player::jump()
{
    if (m_isJumping) return;
    m_isJumping = true;
    body.grounded = false;
    body.velocityY = stats().jumpForce;
}

void Player::attack()
{
    if (!m_ability || m_fireCooldown > 0.0f) return;

    m_isAttacking = true;
    m_hasTriggered = false;
    m_fireCooldown = stats().ability->cooldown;

    if (visual.sprite) visual.sprite->play(SpriteMovementState::ATTACKING);
}

bool Player::wanstToTriggerAbility() const
{
    return m_ability
        && m_isAttacking
        && !m_hasTriggered
        && visual.sprite
        && visual.sprite->currentFrame() >= stats().ability->triggerFrame;
}

void Player::triggerAbility(Scene& scene)
{
    if (!m_ability) return;
    m_ability->trigger(*this, m_facing, scene);
    m_hasTriggered = true;
}

void Player::evCollided(CollisionAxis axis)
{
    if(m_isJumping && axis == CollisionAxis::Y)
    {
        m_isJumping = false;
        body.velocityY = 0.0f;
    }
}

// Editor
std::vector<Property> Player::properties() const
{
    std::vector<Property> properties = GameObject::properties();

    properties.insert(properties.end(), {Property("jumping", m_isJumping, true), Property("grounded", body.grounded, true)});

    return properties;
}
