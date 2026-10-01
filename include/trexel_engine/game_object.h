#pragma once

// Library includes
#include <SDL_render.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <SDL.h>

// Custom includes
#include "common.h"
#include "IRenderable.h"
#include "entity.h"
#include "game_object_def.h"
#include "property.h"
#include "sprite.h"
#include "collision.h"

// C++ includes
#include <optional>
#include <iostream>

class GameObject : public IRenderable, public IInspectable
{
public:
    GameObject(glm::vec2 position, const GameObjectDef& def);
    GameObject() = default;

    // Each object has a unique editor id, so copying will duplicate the id
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    ~GameObject() override = default;

    /*
     * update will be called every frame
     */
    virtual void update(float dt) = 0;

    AABB getBounds() const;
    AABB getBoundsAt(glm::vec2 position) const;

    // IRenderable getters
    glm::vec2 getPosition() const override { return m_position; }
    glm::vec2 getSize() const override { return m_size; }
    std::string getTextureID() const override;
    std::optional<Rect> getSourceRect() const override;
    std::optional<glm::vec3> getColour() const override { return m_colour; }
    double getAngle() const override { return m_angle; }
    Flip getFlip() const override { return m_flip; }

    // GameObject getters
    bool isVisible() const { return m_isVisible; }
    bool isActive() const { return m_isActive; }
    bool isGravity() const { return m_applyGravity; }

    // Collision
    bool hasCollider() const { return m_collider.has_value(); }
    const std::optional<Collider>& getCollider() const { return m_collider; }
    void addCollider(Collider c) { m_collider = c; }
    virtual void evCollided(CollisionAxis axis) {}

    // IRenderable setters
    void setPosition(glm::vec2 position) { m_position = position; }
    void setSize(glm::vec2 size) { m_size = size; }
    void setAngle(double angle) { m_angle = angle; }
    void setFlip(Flip flip) { m_flip = flip; }

    // GameObject setters
    void set_isVisible(bool visible) { m_isVisible = visible; }
    void setIsActive(bool active) { m_isActive = active; }

    // previous positioning
    glm::vec2 getPrevPosition() const { return m_prevPosition; }
    void setPrevPosition(glm::vec2 p) { m_prevPosition = p; }

    // IInspectable
    EntityId entityId() const override { return m_id; }
    std::string typeName() const override { return "GameObject"; }
    std::vector<Property> properties() const override;
    void setProperty(std::string name, const PropertyValue& value) override;

    // Entity id
    EntityId getEntityId() const { return m_id; }

    // Gravity
    void applyGravity(float dt)
    {
        if (!m_applyGravity) return;

        if (m_isGrounded && m_velocityY >= 0.0f)
        {
            m_velocityY = 0.0f;
            return;
        }

        m_velocityY += GRAVITY * dt;
        if (m_velocityY > MAX_FALL_SPEED) m_velocityY = MAX_FALL_SPEED;
        m_position.y += m_velocityY * dt;
    }

    bool isGrounded() const { return m_isGrounded; }
    void setGrounded(bool grounded) { m_isGrounded = grounded; }

    // object def
    std::string getObjectDefName() const { return m_objectDefName; }
    std::string getClassName() const { return m_className; }

protected:
    std::optional<Sprite> m_sprite;
    std::optional<Collider> m_collider;

    bool m_isVisible = true;
    bool m_isActive = true;
    bool m_applyGravity = false;
    bool m_isGrounded = false;

    glm::vec2 m_position = glm::vec2(0.0f);
    glm::vec2 m_size = glm::vec2(0.0f);
    std::string m_textureId;
    std::optional<glm::vec3> m_colour;
    double m_angle = 0.0;
    Flip m_flip = Flip::NONE;

    glm::vec2 m_prevPosition;

    float m_velocityY = 0.0f;
    static constexpr float GRAVITY = 1500.0f;
    static constexpr float MAX_FALL_SPEED = 1000.0f;

    // object def
    std::string m_objectDefName;
    std::string m_className;

    void updateSprite(float dt) { if (m_sprite) m_sprite->update(dt); }

private:
    const EntityId m_id = allocateEntityId();
};
