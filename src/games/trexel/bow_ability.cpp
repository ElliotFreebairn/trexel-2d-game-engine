#include "bow_ability.h"
#include "common.h"
#include "ecs/components.h"
#include "ecs/registry.h"
#include "entity.h"
#include "scene.h"

void BowAbility::trigger(Scene& scene, EntityId user, float facing)
{
    Registry& reg = scene.registry();

    const Transform* t = reg.get<Transform>(user);
    if (!t) return;
    const glm::vec2 origin = t->position + t->size * 0.5f; // copy what we need before spawning

    EntityId arrow = scene.spawn("arrow", origin);
    if (arrow == INVALID_ENTITY) return;

    if (Body* body = reg.get<Body>(arrow)) body->velocity = {facing * ARROW_SPEED, 0.0f};
    if (Projectile* p = reg.get<Projectile>(arrow)) p->owner = user;
    if (Transform* at = reg.get<Transform>(arrow)) at->flip = facing < 0.0f ? Flip::HORIZONTAL : Flip::NONE;
}
