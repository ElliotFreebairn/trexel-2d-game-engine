#pragma once

#include "collision.h"
#include "ecs/registry.h"
#include "entity.h"
#include "input_state.h"
#include "object_factory.h"
#include "property.h"
#include "render_command.h"

#include <optional>
#include <string>
#include <vector>

/*
 * Owns the entities of one level (the Registry) and runs the systems over them each frame
 */
class Scene
{
public:
    Scene(std::string name, const ObjectFactory& factory) : m_name(std::move(name)), m_factory(factory) {}
    virtual ~Scene() = default;

    virtual void onEnter()  {}
    virtual void onExit()   {}
    virtual void onPause()  {}
    virtual void onResume() {}

    virtual void update(float dt, const InputState& input);
    virtual void onViewportResized(glm::vec2 newSize) { (void)newSize; }

    // Spawns from a definition, INVALID_ENTITY if there is no such definition
    EntityId spawn(const std::string& defName, glm::vec2 position);

    Registry& registry() { return m_reg; }
    const ObjectFactory& factory() const { return m_factory; }

    std::vector<RenderCommand> getRenderables();

    const std::string& getName() const { return m_name; }
    const std::string& getBackground() const { return m_background; }
    void setBackground(std::string textureId) { m_background = std::move(textureId); }

    // Editor: reads
    std::vector<EntityInfo> getEntities();
    std::optional<InspectorData> inspect(EntityId id);
    EntityId pick(glm::vec2 worldPos);

    // Editor: writes
    void setProperty(EntityId id, const std::string& name, const PropertyValue& value);
    virtual void destroy(EntityId id);
    void setControlled(EntityId id);

private:
    // Visible entities, back to front: sorted by (layer, id)
    // Shared by getRenderables() and pick() so clicking always selects what's drawn on top
    std::vector<EntityId> drawOrder();

    Registry m_reg;
    std::vector<CollisionEvent> m_collisions; // this frame's, cleared at the start of update()

    std::string m_name;
    std::string m_background;
    const ObjectFactory& m_factory;
};
