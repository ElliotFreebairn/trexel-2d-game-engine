#include "bow_item.h"
#include "scene.h"
#include "player.h"
#include "arrow.h"

void BowItem::onUse(Player& player, Scene& scene)
{
    const glm::vec2 direction = player.getFacing();

    const glm::vec2 origin = player.getPosition() + player.getSize() * 0.5f;

    scene.createObject<Arrow>(origin, direction * ARROW_SPEED, player.getEntityId());
}
