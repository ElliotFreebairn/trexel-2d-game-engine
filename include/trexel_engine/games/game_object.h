#pragma once

// Library includes
#include <glm/glm.hpp>

// Custom includes
#include "ISerializer.h"
#include "common.h"
#include "IRenderable.h"
#include "entity.h"
#include "game_object_def.h"
#include "property.h"
#include "sprite.h"
#include "collision.h"
#include "input_state.h"

// C++ includes
#include <optional>
#include <string>

struct Transform
{
    glm::vec2 position      {0.0f};
    glm::vec2 size          {0.0f};
    double angle            = 0.0;
    Flip flip               = Flip::NONE;
};

struct Body
{
    glm::vec2 velocity {0.0f};
    bool gravity    = false;
    bool grounded   = false; // changed only be scene
};

struct Visual
{
    std::string textureId;
    std::optional<Sprite> sprite;
    std::optional<glm::vec3> colour;
    bool visible = true;
};

class Scene;
struct FrameContext
{
    float dt;
    const InputState& input;
    Scene& scene;
};

/*
 * Base for everything that lives in a Scene
 *
 * Don't create these directly, spawn them through the scene so the definition is looked up for you:
 *      scene.spawn("grass_block", position);
 *      Arrow* arrow = scene.spawn<Arrow>("arrow", position);
 *
 * Subclasses need a (const GameObjectDef& def, glm::vec2 position) constructor
 */
class GameObject : public IRenderable, public IInspectable, public ISerializer
{
public:
    GameObject(const GameObjectDef& def, glm::vec2 position);

    // Each object has a unique editor id, so copying will duplicate the id
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    ~GameObject() override = default;

    /*
     * update will be called every frame
     */
    virtual void update(const FrameContext& ctx) { (void)ctx; }
    virtual void evCollided(const CollisionInfo& hit) { (void)hit; }
    virtual SpriteMovementState animationState() { return SpriteMovementState::IDLE; }

    // State, grouped by what it's for
    Transform transform;
    Body body;
    Visual visual;
    std::optional<Collider> collider;

    bool isActive() const { return m_isActive; }
    void destroy() { m_isActive = false; } // Scene::update() prunes inactive objects

    // The definition this object was built from
    const GameObjectDef& def() const { return *m_def; }
    const std::string& defName() const { return m_def->name; }

    // Collision
    AABB getBounds() const override;
    bool hasCollider() const override { return collider.has_value(); }

    // IRenderable
    glm::vec2 getPosition() const override { return transform.position; }
    glm::vec2 getSize() const override { return transform.size; }
    std::string getTextureID() const override;
    std::optional<Rect> getSourceRect() const override;
    std::optional<glm::vec3> getColour() const override { return visual.colour; }
    double getAngle() const override { return transform.angle; }
    Flip getFlip() const override { return transform.flip; }

    // IInspectable
    EntityId entityId() const override { return m_id; }
    std::string typeName() const override { return "GameObject"; }
    std::vector<Property> properties() const override;
    void setProperty(std::string name, const PropertyValue& value) override;

    // ISerializer
    // Only saves what can change per object, everything else comes from the definition
    // Subclasses that save extra state should call the base version first
    nlohmann::json serialize() const override;
    void deSerialize(const nlohmann::json& j) override;

private:
    const EntityId m_id = allocateEntityId();
    const GameObjectDef* m_def;
    bool m_isActive = true;
};
