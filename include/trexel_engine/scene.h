#pragma once

#include "IRenderable.h"
#include "entity.h"
#include "game_actions.h"
#include "game_object.h"
#include "property.h"
#include <vector>
#include <string>
#include <memory>

class Scene
{
public:
    explicit Scene(std::string name) : m_name(std::move(name)) {};
    virtual ~Scene() = default;

    virtual void onEnter() {};
    virtual void onExit() {};
    virtual void onPause() {};
    virtual void onResume() {};

    virtual void update(float dt);

    std::vector<const IRenderable*> getRenderables() const;

    template <typename T, typename... Args>
    T& createObject(Args&&... args) // Args&&... forwarding references
    {
        static_assert(std::is_base_of_v<GameObject, T>, "T must derive from game object");
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *obj;
        m_objects.push_back(std::move(obj));
        return ref;
    }

    const std::string& getName() const { return m_name; }
    const std::string& getBackground() const { return m_background; }

    void setBackground(std::string textureId) { m_background = std::move(textureId); }

    // Edtior: read-only queries
    std::vector<EntityInfo> getEntities() const;
    IInspectable* findInspectable(EntityId id) const;
    EntityId  pick(glm::vec2 worldPos) const;

    // Editor: changes
    virtual void spawn(const std::string& prototype, glm::vec2 position) { (void)prototype; (void)position; }
    virtual void destroy(EntityId id);

protected:
    std::vector<std::unique_ptr<GameObject>> m_objects;

    void resolveCollisions(GameObject& mover);

private:
    std::string m_name;
    std::string m_background = "";

    void pruneInactive();
};
