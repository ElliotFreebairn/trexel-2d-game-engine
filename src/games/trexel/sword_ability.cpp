#include "sword_ability.h"
#include "collision.h"
#include "game_object.h"
#include "scene.h"

#include <iostream>

void SwordAbility::trigger(GameObject& user, glm::vec2 facing, Scene& scene)
{
    const AABB body = user.getBounds();

    // The hitbox spans the user's height and extends REACH pixels out from whichever side they face
    AABB hitbox{ body.min, body.max };
    if (facing.x >= 0.0f)
    {
        hitbox.min.x = body.max.x;
        hitbox.max.x = body.max.x + REACH;
    }
    else
    {
        hitbox.min.x = body.min.x - REACH;
        hitbox.max.x = body.min.x;
    }

    for (const auto& objPtr : scene.getObjects())
    {
        GameObject& other = *objPtr;
        if (&other == &user || !other.isActive() || !other.hasCollider()) continue;

        if (aabbOverlap(hitbox, other.getBounds()))
        {
            // TODO: apply damage once objects have health
            std::cout << "SwordAbility: hit \"" << other.defName() << "\" (entity " << other.entityId() << ")\n";
        }
    }
}
