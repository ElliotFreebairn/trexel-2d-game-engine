#pragma once

#include <SDL.h>
#include "game_object_def.h"
#include "imgui.h"
#include "asset.h"

struct EditorContext;

// Every object that needs to be stored between frames, let's say for editing values of an object
// use this format, so that the there is an temporary object but also a strucuted way of storing
// id's for which elements are browsing through
struct GameObjectDefDraft
{
    GameObjectDef def;

    std::string animTypeOptionSelected;
};

class UIManager
{
public:
    UIManager(SDL_Window* window, SDL_Renderer* renderer);
    ~UIManager();

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    void process_event(const SDL_Event& e);

    void begin_frame();
    void render();
    void build(EditorContext& ctx, SDL_Texture* world_texture);
    void draw(SDL_Renderer* renderer);

    ImVec2 viewport_size() const { return m_viewportSize; }

    bool wantsKeyboard() const; // when UI widget should get keyboard input
private:
    void drawToolbar(EditorContext& ctx);
    void drawSceneManager(EditorContext& ctx);
    void drawDataManager(EditorContext& ctx);
    void drawSceneBuilder(EditorContext& ctx);
    void drawHierarchy(EditorContext& ctx);
    void drawInspector(EditorContext& ctx);
    void drawAssetManager(EditorContext& ctx);
    void drawDefinitionsManager(EditorContext& ctx);
    void drawFileTree(EditorContext& ctx);
    void drawViewport(EditorContext& ctx, SDL_Texture* world_texture);
    void drawMainEditorWindow(EditorContext& ctx);
    void drawAnimationsManager(EditorContext& ctx, GameObjectDefDraft& draft);

    ImVec2 m_viewportSize = ImVec2(0.0f, 0.0f);

    GameObjectDefDraft m_gameObjectDefDraft;
    Asset m_assetDraft;

    std::string m_selectedAnimationState;
};
