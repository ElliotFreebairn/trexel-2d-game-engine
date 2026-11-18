#pragma once

#include "game_object.h"
#include "entity.h"

class Arrow : public GameObject
{
public:
    Arrow(const GameObjectDef& def, glm::vec2 position);

    // Call after spawning to send the arrow flying
    void launch(glm::vec2 velocity, EntityId ownerId);

    void update(float dt) override;
    void evCollided(const CollisionInfo& hit) override;

    std::string typeName() const override { return "Arrow"; }

    EntityId ownerId() const { return m_ownerId; }

private:
    glm::vec2 m_velocity = glm::vec2(0.0f);
    float m_lifetime = 4.0f; // despawns after this long
    EntityId m_ownerId = INVALID_ENTITY;
};
