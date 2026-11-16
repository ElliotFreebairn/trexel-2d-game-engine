#include "bow_ability.h"
#include "arrow.h"
#include "scene.h"
#include "game_object.h"

void BowAbility::trigger(GameObject &user, glm::vec2 facing, Scene &scene)
{
    const glm::vec2 origin = user.getPosition() + user.getSize() * 0.5f;

    if (Arrow* arrow = scene.spawn<Arrow>("arrow", origin))
    {
        arrow->launch(facing * ARROW_SPEED, user.entityId());
    }
}
