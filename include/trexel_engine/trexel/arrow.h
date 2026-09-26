#pragma once

#include "../game_object.h"
#include "entity.h"

class Arrow : public GameObject
{
public:
    Arrow(glm::vec2 position, glm::vec2 velocity, EntityId ownerId);

    void update(float dt) override;
    void evCollided(CollisionAxis axis) override;

    std::string typeName() const override { return "Arrow"; }

    EntityId ownerId() const { return m_ownerId; }

private:
    glm::vec2 m_velocity;
    float m_lifetime = 4.0f; // despawns after this long
    EntityId m_ownerId;
};
