#include "arrow.h"
#include "entity.h"
#include "game_object.h"

Arrow::Arrow(const GameObjectDef& def, glm::vec2 position)
    : GameObject(def, position)
{
}

void Arrow::launch(glm::vec2 velocity, EntityId ownerId)
{
    body.velocity = velocity;
    m_ownerId = ownerId;
}

void Arrow::update(const FrameContext& ctx)
{
    m_lifetime -= ctx.dt;
    if (m_lifetime <= 0.0f) destroy();
}

void Arrow::evCollided(const CollisionInfo& hit)
{
    if (hit.other->entityId() == m_ownerId) return;
    if (!hit.other->collider->isSolid) return; // ignore other arrows / triggers
    destroy();
}
