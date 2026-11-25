#pragma once

#include "../games/ability.h"
#include "../games/collision.h"
#include "../games/entity.h"
#include "../sprite/sprite.h"
#include "../utilities/common.h"
#include "../ability_registry.h"

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>


// --- CORE (engine) ---

struct Transform
{
    glm::vec2 position {0.0f};
    glm::vec2 size {0.0f};
    double angle = 0.0;
    Flip flip = Flip::NONE;

    template <typename F>
    void fields(F&& f)
    {
        f("position", position);
        f("size", size);
        f("angle", angle);
        f("flip", flip);
    }
};

struct Body
{
    glm::vec2 velocity {0.0f};
    bool gravity = false;
    bool grounded = false; // written only by physicsSystem

    template <typename F>
    void fields(F&& f)
    {
        f("velocity", velocity);
        f("gravity", gravity);
        f("grounded", grounded);
    }
};

struct Visual
{
    std::string textureId;
    std::optional<Sprite> sprite;
    std::optional<glm::vec3> colour;
    std::unordered_map<SpriteMovementState, SpriteAnimation> animations;
    bool visible = true;
    int layer = 0; // higher draws on top; ties broken by spawn order

    template <typename F>
    void fields(F&& f)
    {
        f("textureId", textureId);
        //f("sprite", sprite);
        f("colour", colour);
        // animation should probably be a component or std::optional
        // f("animation", animation)
        f("visible", visible);
        f("layer", layer);
        // figure out what to do with std::optional?
    }
};

// Which prefab (definition) built this. Used for saving and the editor
struct Prefab
{
    std::string name;

    template <typename F>
    void fields(F&& f)
    {
        f("name", name);
    }
};

// --- Gameplay ---
// What the entity WANTS to do this frame. Brains write it, bodies read it
// do we differentiate what components have state or what fields?
struct Intent
{
    float   moveX   = 0.0f;
    bool    jump    = false;
    bool    attack  = false;

    template <typename F>
    void fields(F&& f)
    {
        f("moveX", moveX);
        f("jump", jump);
        f("attack", attack);
    }
};

// Brains (tags)
struct PlayerControlled {};
struct PatrolAI {};

// Replaces CharacterStats
struct Movement
{
    float moveSpeed = 200.0f;
    float jumpForce = -500.0f;

    template <typename F>
    void fields(F&& f)
    {
        f("moveSpeed", moveSpeed);
        f("jumpForce", jumpForce);
    }
};

struct Collider
{
    bool isSolid = true;
    glm::vec2 size {0.0f, 0.0f};
    glm::vec2 offset {0.0f, 0.0f};

    template <typename F>
    void fields(F&& f)
    {
        f("isSolid", isSolid);
        f("size", size);
        f("offset", offset);
    }
};

struct Attack
{
    std::shared_ptr<IAbility> ability; // abilities are stateless so can be shared
    float cooldown = 0.4f;
    int triggerFrame = 0; // frame of ATTACKING animation where ability fires

    // run time state
    float timer = 0.0f;
    bool active = false;
    bool triggered = false;

    template <typename F>
    void fields(F&& f)
    {
        // f("ability", ability) // what do we do with this
        f("cooldown", cooldown);
        f("triggerFrame", triggerFrame);
        f("timer", timer);
        // should we allow for editable state fields?
        f("timer", timer);
        f("active", active);
        f("triggered", triggered);
    }
};

struct AbilityDef
{
    std::string name;
    float cooldown = 0.4f;
    int triggerFrame = 0; // frame of the ATTACKING animation where the ability fires

    AbilityRegistry::Maker make; // resolved from name when loaded, never saved

    template <typename F>
    void fields(F&& f)
    {
        f("name", name);
        f("cooldown", cooldown);
        f("triggerFrame", triggerFrame);
    }
};

struct Projectile
{
    float lifetime = 4.0f;
    EntityId owner = INVALID_ENTITY;

    template <typename F>
    void fields(F&& f)
    {
        f("lifetime", lifetime);
        //f("owner", owner); // what do to do?
    }
};

struct Health
{
    int current = 3;
    int max = 3;

    template <typename F>
    void fields(F&& f)
    {
        f("current", current);
        f("max", max);
    }
};

struct DamageOnHit
{
    int amount = 1;

    template <typename F>
    void fields(F&& f)
    {
        f("amount", amount);
    }
};

// --- Helpers ---

// Facing is derived from flip, not stored separately
inline float facingX(const Transform& t)
{
    return t.flip == Flip::HORIZONTAL ? -1.0f : 1.0f;
}

// World-space box. Uses the collider if there is one, otherwise the whole sprite
inline AABB worldBounds(const Transform& t, const Collider* c)
{
    if (!c) return {t.position, t.position + t.size};

    float offsetX = c->offset.x;
    if (t.flip == Flip::HORIZONTAL)
    {
        offsetX = t.size.x - c->offset.x - c->size.x; // mirror the collider with the sprite
    }

    const glm::vec2 min = t.position + glm::vec2(offsetX, c->offset.y);
    return {min, min + c->size};
}
