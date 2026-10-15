#include "bow_item.h"
#include "scene.h"
#include "player.h"
#include "arrow.h"

void BowItem::onUse(Player& player, Scene& scene)
{
    const glm::vec2 direction = player.getFacing();

    const glm::vec2 origin = player.getPosition() + player.getSize() * 0.5f;

    if (Arrow* arrow = scene.spawn<Arrow>("arrow", origin))
    {
        arrow->launch(direction * ARROW_SPEED, player.entityId());
    }
}
