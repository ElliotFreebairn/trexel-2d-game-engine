#include "player.h"
#include "bow_item.h"
#include "collision.h"
#include "common.h"
#include "game_object.h"
#include "game_object_def.h"
#include "input_state.h"
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

void Player::update(const FrameContext& ctx)
{
    const InputState& in = ctx.input;

    // intent -> velocity. scene does the moving
    body.velocity.x = in.move.x * stats().moveSpeed;
    if (in.move.x != 0.0f)
    {
        m_facing.x = in.move.x < 0.0f ? -1.0f : 1.0f;
        transform.flip = m_facing.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE;
    }

    if (in.jumpPressed && body.grounded) // grounded = end of last frame's physics
        body.velocity.y = stats().jumpForce;

    m_cooldown = std::max(0.0f, m_cooldown - ctx.dt);
    if (in.firePressed && m_ability && m_cooldown == 0.0f)
    {
        m_isAttacking = true;
        m_hasTriggered = false;
        m_cooldown = stats().ability->cooldown;
        // start now so currentFrame() below is the attack's
        if (visual.sprite) visual.sprite->play(SpriteMovementState::ATTACKING);
    }

    if (m_isAttacking && visual.sprite)
    {
        if (!m_hasTriggered && visual.sprite->currentFrame() >= stats().ability->triggerFrame)
        {
            m_ability->trigger(*this, m_facing, ctx.scene);
            m_hasTriggered = true;
        }
        if (visual.sprite->isFinished()) m_isAttacking = false;
    }
}

SpriteMovementState Player::animationState()
{
    if (m_isAttacking)             return SpriteMovementState::ATTACKING;
    if (!body.grounded)            return SpriteMovementState::JUMPING;
    if (body.velocity.x != 0.0f)   return SpriteMovementState::WALKING;
    return SpriteMovementState::IDLE;
}

std::vector<Property> Player::properties() const
{
    std::vector<Property> properties = GameObject::properties();

    properties.insert(properties.end(),{Property("grounded", body.grounded, true)});

    return properties;
}
