#include "ecs/systems.h"

#include "ecs/components.h"
#include "ecs/registry.h"
#include "input_state.h"
#include "scene.h"

#include <algorithm>

namespace
{
constexpr float GRAVITY        = 1500.0f;
constexpr float MAX_FALL_SPEED = 1000.0f;
}

// --- Brains ---

void playerInputSystem(Registry& reg, const InputState& input)
{
    reg.each<PlayerControlled, Intent>([&](EntityId, PlayerControlled&, Intent& intent) {
        intent.moveX  = input.move.x;
        intent.jump   = input.jumpPressed;
        intent.attack = input.firePressed;
    });
}

void patrolAISystem(Registry& reg)
{
    reg.each<PatrolAI, Intent, Transform>([&](EntityId e, PatrolAI&, Intent& intent, Transform& t) {
        if (reg.has<PlayerControlled>(e)) return; // possessed: the player drives
        intent.moveX = facingX(t);
    });
}

// --- Bodies ---

void movementSystem(Registry& reg)
{
    reg.each<Intent, Movement, Body, Transform>(
        [](EntityId, Intent& intent, Movement& movement, Body& body, Transform& t) {
            body.velocity.x = intent.moveX * movement.moveSpeed;

            if (intent.moveX != 0.0f)
                t.flip = intent.moveX < 0.0f ? Flip::HORIZONTAL : Flip::NONE;

            if (intent.jump && body.grounded) // grounded = result of last frame's physics
                body.velocity.y = movement.jumpForce;
        });
}

void attackSystem(Scene& scene, float dt)
{
    Registry& reg = scene.registry();

    reg.each<Attack, Intent, Transform>([&](EntityId e, Attack& atk, Intent& intent, Transform& t) {
        if (!atk.ability) return;
        atk.timer = std::max(0.0f, atk.timer - dt);

        // Only wait on the animation if there is an attack animation to wait on
        Sprite* sprite = nullptr;
        if (Visual* vis = reg.get<Visual>(e);
            vis && vis->sprite && vis->sprite->hasAnimation(SpriteMovementState::ATTACKING))
        {
            sprite = &*vis->sprite;
        }

        if (intent.attack && !atk.active && atk.timer == 0.0f)
        {
            atk.timer = atk.cooldown;
            if (!sprite)
            {
                atk.ability->trigger(scene, e, facingX(t)); // no animation: fire immediately
                return;
            }
            atk.active    = true;
            atk.triggered = false;
            sprite->play(SpriteMovementState::ATTACKING); // start now so currentFrame() is the attack's
        }

        if (!atk.active || !sprite) return;

        if (!atk.triggered && sprite->currentFrame() >= atk.triggerFrame)
        {
            // May spawn entities. Safe: that adds to other pools, not the Attack pool being looped
            atk.ability->trigger(scene, e, facingX(t));
            atk.triggered = true;
        }
        if (sprite->isFinished()) atk.active = false;
    });
}

// --- Physics ---

namespace
{
void moveAxis(Registry& reg, EntityId self, Transform& t, Body& body, glm::vec2 delta,
              std::vector<CollisionEvent>& out)
{
    if (delta == glm::vec2(0.0f)) return;
    t.position += delta;

    const Collider* selfCollider = reg.get<Collider>(self);
    if (!selfCollider || !selfCollider->isSolid) return; // triggers aren't pushed

    reg.each<Collider, Transform>([&](EntityId other, Collider& oc, Transform& ot) {
        if (other == self || !oc.isSolid) return;

        const AABB a = worldBounds(t, selfCollider); // recomputed: earlier pushes moved us
        const AABB b = worldBounds(ot, &oc);
        if (!aabbIntersects(a, b)) return;

        // We moved along exactly one axis, so the side we hit is known
        glm::vec2 normal {0.0f};
        if      (delta.x > 0) { t.position.x -= a.max.x - b.min.x; normal = {-1, 0}; }
        else if (delta.x < 0) { t.position.x += b.max.x - a.min.x; normal = { 1, 0}; }
        else if (delta.y > 0) { t.position.y -= a.max.y - b.min.y; normal = { 0,-1}; body.grounded = true; }
        else                  { t.position.y += b.max.y - a.min.y; normal = { 0, 1}; }

        (normal.x != 0.0f ? body.velocity.x : body.velocity.y) = 0.0f;
        out.push_back({self, other, normal});
    });
}

void reportOverlaps(Registry& reg, std::vector<CollisionEvent>& out)
{
    // Flatten first so each PAIR is visited once (i < j), which each() can't express
    struct Entry { EntityId id; AABB box; bool solid; };
    std::vector<Entry> entries;
    reg.each<Collider, Transform>([&](EntityId e, Collider& c, Transform& t) {
        entries.push_back({e, worldBounds(t, &c), c.isSolid});
    });

    for (size_t i = 0; i < entries.size(); ++i)
    {
        for (size_t j = i + 1; j < entries.size(); ++j)
        {
            const Entry& a = entries[i];
            const Entry& b = entries[j];
            if (a.solid && b.solid) continue; // physics already reported these
            if (!aabbIntersects(a.box, b.box)) continue;

            out.push_back({a.id, b.id, {0, 0}}); // tell both sides
            out.push_back({b.id, a.id, {0, 0}});
        }
    }
}
}

void physicsSystem(Registry& reg, float dt, std::vector<CollisionEvent>& collisions)
{
    reg.each<Body, Transform>([&](EntityId e, Body& body, Transform& t) {
        if (body.gravity)
            body.velocity.y = std::min(body.velocity.y + GRAVITY * dt, MAX_FALL_SPEED);

        body.grounded = false; // decided again every frame by moveAxis
        moveAxis(reg, e, t, body, {body.velocity.x * dt, 0.0f}, collisions);
        moveAxis(reg, e, t, body, {0.0f, body.velocity.y * dt}, collisions);
    });

    reportOverlaps(reg, collisions);
}

// --- Reactions ---

void patrolBounceSystem(Registry& reg, const std::vector<CollisionEvent>& collisions)
{
    for (const CollisionEvent& hit : collisions)
    {
        if (hit.normal.x == 0.0f || !reg.has<PatrolAI>(hit.self)) continue;
        if (Transform* t = reg.get<Transform>(hit.self))
            t->flip = hit.normal.x < 0.0f ? Flip::HORIZONTAL : Flip::NONE; // face away from the wall
    }
}

void projectileSystem(Registry& reg, float dt, const std::vector<CollisionEvent>& collisions)
{
    reg.each<Projectile>([&](EntityId e, Projectile& p) {
        p.lifetime -= dt;
        if (p.lifetime <= 0.0f) reg.destroy(e);
    });

    for (const CollisionEvent& hit : collisions)
    {
        const Projectile* p = reg.get<Projectile>(hit.self);
        if (!p || hit.other == p->owner) continue;

        const Collider* c = reg.get<Collider>(hit.other);
        if (c && c->isSolid) reg.destroy(hit.self); // ignore other arrows / triggers
    }
}

void damageSystem(Registry& reg, const std::vector<CollisionEvent>& collisions)
{
    for (const CollisionEvent& hit : collisions)
    {
        const DamageOnHit* dmg = reg.get<DamageOnHit>(hit.self);
        Health* hp = reg.get<Health>(hit.other);
        if (!dmg || !hp) continue;

        if (const Projectile* p = reg.get<Projectile>(hit.self); p && p->owner == hit.other)
            continue; // don't shoot yourself

        hp->current -= dmg->amount;
    }
}

void deathSystem(Registry& reg)
{
    reg.each<Health>([&](EntityId e, Health& hp) {
        if (hp.current <= 0) reg.destroy(e);
    });
}

// --- Visuals ---

void animationSystem(Registry& reg, float dt)
{
    reg.each<Visual>([&](EntityId e, Visual& vis) {
        if (!vis.sprite) return;

        const Attack* atk = reg.get<Attack>(e);
        const Body* body  = reg.get<Body>(e);

        SpriteMovementState state = SpriteMovementState::IDLE;
        if (atk && atk->active)                            state = SpriteMovementState::ATTACKING;
        else if (body && body->gravity && !body->grounded) state = SpriteMovementState::JUMPING;
        else if (body && body->velocity.x != 0.0f)         state = SpriteMovementState::WALKING;

        vis.sprite->play(state); // a state with no animation is ignored, the current one keeps playing
        vis.sprite->update(dt);
    });
}
