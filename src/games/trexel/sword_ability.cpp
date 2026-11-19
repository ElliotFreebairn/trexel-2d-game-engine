#include "sword_ability.h"
#include "collision.h"
#include "ecs/components.h"
#include "ecs/registry.h"
#include "scene.h"

void SwordAbility::trigger(Scene& scene, EntityId user, float facing)
{
    Registry& reg = scene.registry();

    const Transform* t = reg.get<Transform>(user);
    if (!t) return;
    const AABB body = worldBounds(*t, reg.get<Collider>(user));

    // Spans the user's height, extends REACH out from whichever side they face
    AABB hitbox{body.min, body.max};
    if (facing >= 0.0f) { hitbox.min.x = body.max.x;          hitbox.max.x = body.max.x + REACH; }
    else                { hitbox.min.x = body.min.x - REACH;  hitbox.max.x = body.min.x; }

    // Only things with Health can be hurt, so only query those
    reg.each<Health, Transform>([&](EntityId other, Health& hp, Transform& ot) {
        if (other == user) return;
        if (aabbIntersects(hitbox, worldBounds(ot, reg.get<Collider>(other))))
            hp.current -= DAMAGE;
    });
}
