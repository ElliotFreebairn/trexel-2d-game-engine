#include "arrow.h"
#include "entity.h"

Arrow::Arrow(const GameObjectDef& def, glm::vec2 position)
    : GameObject(def, position)
{
}

void Arrow::launch(glm::vec2 velocity, EntityId ownerId)
{
    m_velocity = velocity;
    m_ownerId = ownerId;
}

void Arrow::update(float dt)
{
    transform.position += m_velocity * dt;

    m_lifetime -= dt;
    if (m_lifetime <= 0.0f)
    {
        destroy(); // Scene::update() prunes inactive objects
    }
}

void Arrow::evCollided(const CollisionInfo& hit)
{
    (void)hit;
    destroy();
}
