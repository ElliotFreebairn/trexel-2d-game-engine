#include "arrow.h"
#include "entity.h"
#include "object_defs.h"

Arrow::Arrow(glm::vec2 position, glm::vec2 velocity, EntityId onwerId)
    : GameObject(position, ObjectDefs::getObjectDefinition("Arrow"))
    , m_velocity(velocity), m_ownerId(onwerId)
{
}

void Arrow::update(float dt)
{
    setPosition(getPosition() + m_velocity * dt);

    m_lifetime -= dt;
    if (m_lifetime <= 0.0f)
    {
        setIsActive(false); // Scene::update() prunes inactive objects
    }
}

void Arrow::evCollided(CollisionAxis axis)
{
    (void)axis;
    setIsActive(false);
}
