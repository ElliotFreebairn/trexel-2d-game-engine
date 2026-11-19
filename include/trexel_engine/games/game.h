#pragma once

// Library includes
#include "IRenderable.h"

// Custom includes
#include "asset_manager.h"
#include "editor_command.h"
#include "entity.h"
#include "game_actions.h"
#include "object_factory.h"
#include "input_state.h"
#include "main_scene.h"
#include "property.h"
#include "scene.h"
#include "scene_manager.h"
#include "scene_object.h"

// C++ includes
#include <cerrno>
#include <filesystem>
#include <vector>


/*
 * A game knows nothing about the renderer
 */
class Game
{
public:
    friend class Scene;

    virtual ~Game() = default;

    void init()
    {
        intialiseAssets(m_assetManager);
        onInit();
    }

    virtual void onUpdate(float deltaTime, const InputState& input)
    {
        m_sceneManager.update(deltaTime, input);
    }

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
                scene->spawn(command.prototype, command.position);
                break;
            case EditorCommandType::DestroyEntity:
                m_sceneManager.current()->destroy(command.entity);
                break;
            case EditorCommandType::SetPlayMode:
                break; // owned by the game engine
            case EditorCommandType::AddAsset:
                if (auto result = m_assetManager.addAsset(command.asset); !result)
                    std::cerr << "Add Asset Failed: " << describe(result.error()) << "\n";
                else
                    m_assetManager.saveManifest();
                break;
            case EditorCommandType::AddObjectDefinition:
                if(bool result = m_objectFactory.addDefinition(command.objectDef); !result)
                    std::cerr << "Add Object Def Failed!\n";
                else
                    m_objectFactory.saveDefinitions("data/definitions/new_objects.json");
        }
    }

    // misc
    const AssetManager& getAssetManager() const { return m_assetManager; }
    const ObjectFactory& getObjectFactory() const { return m_objectFactory; }
    bool isRunning() const { return m_isRunning; }


    glm::vec2 getViewport() const { return m_viewport; }
    void setViewport(glm::vec2 viewport)
    {
        m_viewport = viewport;
        if (Scene* scene = m_sceneManager.current())
        {
            scene->onViewportResized(viewport);
        }
    }


protected:
    virtual void intialiseAssets(AssetManager& manager) = 0;

    virtual void registerObjectTypes(ObjectFactory& factory) { (void)factory; }
    virtual void onInit() = 0;

    AssetManager m_assetManager;
    ObjectFactory m_objectFactory;
    SceneManager m_sceneManager;

    bool m_isRunning = true;

    glm::vec2 m_viewport = glm::vec2(0.0f);
};
