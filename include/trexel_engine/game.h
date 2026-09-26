#pragma once

// Library includes
#include "IRenderable.h"

// Custom includes
#include "asset_manager.h"
#include "editor_command.h"
#include "entity.h"
#include "game_actions.h"
#include "input_state.h"
#include "property.h"
#include "scene.h"
#include "scene_manager.h"
#include "scene_object.h"

// C++ includes
#include <cerrno>
#include <vector>

/*
 * A game knows nothing about the renderer
 */
class Game
{
public:
    virtual ~Game() = default;

    void init()
    {
        intialiseAssets(m_assetManager);
        onInit();
    }

    virtual void onUpdate(float deltaTime, const InputState& input)
    {
        (void)input;
        m_sceneManager.update(deltaTime);
    }
    virtual void onShutdown() = 0;

    // Rendering
    // game bundles up the visual state of the world into a dumb data structure
    virtual std::vector<const IRenderable*> getRenderables() { return m_sceneManager.getRenderables(); }

    std::string getBackground() const
    {
        Scene* currentScene = m_sceneManager.current();
        return currentScene ? currentScene->getBackground() : "";
    }

    // Editor: read-only queries
    std::vector<EntityInfo> getEntities() const
    {
        Scene* scene = m_sceneManager.current();
        return scene ? scene->getEntities() : std::vector<EntityInfo>{};
    }

    const IInspectable* findInspectable(EntityId id) const
    {
        Scene* scene = m_sceneManager.current();
        return scene ? scene->findInspectable(id) : nullptr;
    }

    // Which entity is under this world position
    EntityId pick(glm::vec2 worldPos) const
    {
        Scene* scene = m_sceneManager.current();
        return scene ? scene->pick(worldPos) : INVALID_ENTITY;
    }

    // Editor: the only write path
    void applyCommand(const EditorCommand& command)
    {
        Scene* scene = m_sceneManager.current();
        if (!scene) return;

        switch (command.type)
        {
            case EditorCommandType::SetProperty:
                if (IInspectable* object = scene->findInspectable(command.entity))
                {
                    object->setProperty(command.propertyName, command.propertyValue);
                }
                break;
            case EditorCommandType::SpawnEntity:
                //m_sceneManager.current()->createObject<SceneObject>(command.position, ObjectDefs::GrassBlock());
                break;
            case EditorCommandType::DestroyEntity:
                m_sceneManager.current()->destroy(command.entity);
                break;
            case EditorCommandType::SetPlayMode:
                break; // owned by the game engine
        }
    }

    // misc
    const AssetManager& getAssetManager() const { return m_assetManager; }
    bool isRunning() const { return m_isRunning; }

    glm::vec2 getViewport() { return viewport; }
    void setViewport(glm::vec2 viewport) { this->viewport = viewport; }

protected:
    virtual void intialiseAssets(AssetManager& assets) = 0;
    virtual void onInit() = 0;

    SceneManager m_sceneManager;
    AssetManager m_assetManager;
    bool m_isRunning = true;

private:
    glm::vec2 viewport = glm::vec2(0.0f);
};
