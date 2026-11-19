#pragma once

#include "IRenderable.h"
#include "entity.h"
#include "game_actions.h"
#include "game_object.h"
#include "input_state.h"
#include "object_factory.h"
#include "property.h"
#include <vector>
#include <string>
#include <memory>

class Scene
{
public:
    Scene(std::string name, const ObjectFactory& factory) : m_name(std::move(name)), m_factory(factory) {};
    virtual ~Scene() = default;

    virtual void onEnter() {};
    virtual void onExit() {};
    virtual void onPause() {};
    virtual void onResume() {};

    virtual void update(float dt, const InputState& input);
    virtual void onViewportResized(glm::vec2 newSize) { (void)newSize; }

    std::vector<const IRenderable*> getRenderables() const;

    // Creates an object from its definition and adds it to the scene, nullptr if it couldn't be created
    GameObject* spawn(const std::string& defName, glm::vec2 position);

    // Same as above but returns the class you expect, nullptr (and nothing added) if the definition builds something else
    template <typename T>
    T* spawn(const std::string& defName, glm::vec2 position)
    {
        static_assert(std::is_base_of_v<GameObject, T>, "T must derive from game object");
        std::unique_ptr<GameObject> obj = m_factory.create(defName, position);

        T* typed = dynamic_cast<T*>(obj.get());
        if (!typed) return nullptr;

        addObject(std::move(obj));
        return typed;
    }

    GameObject& addObject(std::unique_ptr<GameObject> obj);
    const std::vector<std::unique_ptr<GameObject>>& getObjects() const { return m_objects; }

    const std::string& getName() const { return m_name; }
    const std::string& getBackground() const { return m_background; }

    void setBackground(std::string textureId) { m_background = std::move(textureId); }

    // Edtior: read-only queries
    std::vector<EntityInfo> getEntities() const;
    IInspectable* findInspectable(EntityId id) const;
    EntityId  pick(glm::vec2 worldPos) const;

    // Editor: changes
    virtual void destroy(EntityId id);

    void setControlled(EntityId id) { m_controlled = id; }

protected:
    std::vector<std::unique_ptr<GameObject>> m_objects;
    std::vector<std::unique_ptr<GameObject>> m_pending;

private:
    EntityId m_controlled = INVALID_ENTITY;
    std::string m_name;
    std::string m_background = "";
    const ObjectFactory& m_factory;
    bool m_inUpdate = false;

    void integrate(GameObject& obj, float dt);
    void moveAxis(GameObject& obj, glm::vec2 delta);
    void reportOverlaps();
    void flushPending();
    void pruneInactive();
};
