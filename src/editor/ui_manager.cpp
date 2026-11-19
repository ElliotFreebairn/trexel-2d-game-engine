#include "ui_manager.h"
#include "IRenderable.h"
#include "editor_command.h"
#include "editor_state.h"
#include "entity.h"
#include "game.h"
#include "game_object_def.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "asset_manager.h"
#include "imgui_stdlib.h"
#include "property.h"
#include "file_tree_widget.h"

#include <glm/glm.hpp>
#include <stdexcept>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

// Draws one property. If the user edits it, emits a SetProperty command.
void drawProperty(EntityId id, const Property& property, CommandQueue& commands)
{
    PropertyValue edited = property.value; // UI edits this local copy
    bool changed = false;
    const char* label = property.name.c_str();

    ImGui::BeginDisabled(property.readOnly);
    if (auto* b = std::get_if<bool>(&edited))
        changed = ImGui::Checkbox(label, b);
    else if (auto* i = std::get_if<int>(&edited))
        changed = ImGui::DragInt(label, i);
    else if (auto* f = std::get_if<float>(&edited))
        changed = ImGui::DragFloat(label, f);
    else if (auto* v = std::get_if<glm::vec2>(&edited))
        changed = ImGui::DragFloat2(label, &v->x);
    else if (auto* s = std::get_if<std::string>(&edited))
        changed = ImGui::InputText(label, s);

        //ImGui::LabelText(label, "%s", s->c_str());
    ImGui::EndDisabled();

    if (changed)
    {
        commands.push(EditorCommand::setProperty(id, property.name, std::move(edited)));
    }
}

UIManager::UIManager(SDL_Window* window, SDL_Renderer* renderer)
{
    if (!window || !renderer)
    {
       throw std::runtime_error("UIManager requires a valid window and renderer");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if(!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer))
    {
        ImGui::DestroyContext();
        throw std::runtime_error("ImGui_ImplSDL2_InitForSDLRenderer failed!");
    }
    if(!ImGui_ImplSDLRenderer2_Init(renderer))
    {
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        throw std::runtime_error("ImGui_ImplSDLRenderer2_Init failed!");
    }
}

UIManager::~UIManager()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::process_event(const SDL_Event& e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
}

bool UIManager::wantsKeyboard() const
{
    return ImGui::GetIO().WantCaptureKeyboard;
}

void UIManager::begin_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport();
}

void UIManager::build(EditorContext& ctx, SDL_Texture* world_texture)
{
    drawMainEditorWindow(ctx);
    drawViewport(ctx, world_texture);
}


void UIManager::drawMainEditorWindow(EditorContext &ctx)
{
    // Create your single clean workspace window
    ImGui::Begin("Workspace", NULL, ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginTabBar("WorkspaceTabBar"))
    {
        // 1. Draw your refactored toolbar tab
        drawToolbar(ctx);

        // 2. Draw your hierarchy tab right next to it
        drawFileTree(ctx);

        // 3. Draw your scene manager next to it
        drawSceneManager(ctx);

        drawInspector(ctx);

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void UIManager::drawToolbar(EditorContext &ctx)
{
    // The parent window and TabBar are handled externally now
    if (ImGui::BeginTabItem("Toolbar"))
    {
        // Use standard spacing instead of bulky CollapsingHeaders for a cleaner look
        ImGui::TextDisabled("GAME CONTROLS");
        if (ctx.state.playing)
        {
            if (ImGui::Button("Stop")) {
                ctx.commands.push(EditorCommand::setPlayMode(false));
            }
        }
        else
        {
            if (ImGui::Button("Play")) {
                ctx.commands.push(EditorCommand::setPlayMode(true));
            }
        }

        ImGui::Separator();
        ImGui::TextDisabled("VISUALS");
        ImGui::Checkbox("Collision Boxes", &ctx.state.showColliders);

        ImGui::EndTabItem();
    }
}

void UIManager::drawSceneManager(EditorContext &ctx)
{
    if (ImGui::BeginTabItem("Scene Manager"))
    {
        // we need a way for the the ui/user to know which scene
        // they are manipulating, such as the main_scene ...
        ImGui::SeparatorText("Scene Builder");
        drawSceneBuilder(ctx);

        // the asset manager is not scene specific, but for now is alright here
        ImGui::SeparatorText("Scene Assets");
        drawAssetManager(ctx);

        // the definition manager
        ImGui::SeparatorText("Scene definitions");
        drawDefinitionsManager(ctx);

        ImGui::PopID();
    }
}

void UIManager::drawSceneBuilder(EditorContext &ctx)
{
    if (ctx.state.sceneBuilding)
    {
        if (ImGui::Button("Stop Scene Building"))
        {
            ctx.state.sceneBuilding = false;
        }
    }
    else
    {
        if (ImGui::Button("Start Scene Building"))
        {
            ctx.state.sceneBuilding = true;
        }
    }

    ImGui::Indent(4.0f);

    // expose the option to choose which definition they would like to spawn in the game
    // that means we need to be able to grab the object definitions from the game
    std::vector<std::string> definitions = ctx.game.getObjectFactory().getDefinitionsKeys();
    static size_t defintionsSelectedIndex = 0;

    if (ImGui::BeginListBox("object definitions to use"))
    {
        for (size_t defIndex = 0; defIndex < definitions.size(); defIndex++)
        {
           const bool isSelected = defintionsSelectedIndex == defIndex;
           if (ImGui::Selectable(definitions.at(defIndex).c_str(), isSelected))
           {
               defintionsSelectedIndex = defIndex;
               ctx.state.defintion = definitions.at(defIndex);
           }
        }
    }

    ImGui::EndListBox();
}

void UIManager::drawDefinitionsManager(EditorContext &ctx)
{
    // rough display a list of definitions
    std::vector<GameObjectDef> definitions = ctx.game.getObjectFactory().getDefinitionsValues();

    if (ImGui::BeginListBox("id"))
    {
        for (const GameObjectDef& def : definitions)
        {
            bool isSelected = def.name == m_selecteDefName;

            if (ImGui::Selectable(def.name.c_str(), isSelected))
            {
                m_selecteDefName = def.name;
                m_defDraft = def;
            }
        }

        ImGui::EndListBox();
    }

    if (m_selecteDefName.empty()) return; // nothing selected

    ImGui::InputText("class", &m_defDraft.className);
    ImGui::InputFloat2("size", &m_defDraft.size.x);
    ImGui::Checkbox("gravity", &m_defDraft.gravity);

    if (m_defDraft.collider.has_value())
    {
        ImGui::InputFloat2("collider size", &m_defDraft.collider->size.x);
        ImGui::InputFloat2("collider offset", &m_defDraft.collider->offset.x);
    }

    int animationsCount = 0;
    if (!m_defDraft.animations.empty())
    {
        for (const auto& [state, animation] : m_defDraft.animations)
        {
           animationsCount++;
        }
    }
    ImGui::Text("Total No. Animations %d", animationsCount);

    if (ImGui::Button("Apply Edit"))
    {
        ctx.commands.push(EditorCommand::addObjectDefinition(m_defDraft));
    }
}

void UIManager::drawAssetManager(EditorContext &ctx)
{
    ImGui::InputText("Asset ID", &m_assetDraft.id);
    ImGui::InputText("Asset Path", &m_assetDraft.filePath);

    // must match AssetType's order
    const char* typeNames[] = {"Texture"};
    int typeIndex = static_cast<int>(m_assetDraft.type);
    if (ImGui::Combo("Asset Type", &typeIndex, typeNames, IM_COUNTOF(typeNames)))
    {
        m_assetDraft.type = static_cast<AssetType>(typeIndex);
    }

    // same rules addAsset enforces, used here for live feedback
    const auto check = ctx.game.getAssetManager().validate(m_assetDraft);
    if (!check)
    {
        const std::string_view msg = describe(check.error());
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%.*s", static_cast<int>(msg.size()), msg.data());
    }

    ImGui::BeginDisabled(!check.has_value());
    if (ImGui::Button("Add Asset To Manifest"))
    {
        ctx.commands.push(EditorCommand::addAsset(m_assetDraft));  // copies the draft into the command
        m_assetDraft = {};                                         // reset the form
    }
    ImGui::EndDisabled();
}

void UIManager::drawInspector(EditorContext &ctx)
{
    if (ImGui::BeginTabItem("Inspector"))
    {
        // works for any game, the UI only knows IInspectable
        const IInspectable* object = ctx.game.findInspectable(ctx.state.selected);
        if (!object)
        {
            ImGui::TextUnformatted("Nothing Selected");
        }
        else
        {
            ImGui::Text("%s (id %u)", object->typeName().c_str(), static_cast<unsigned>(object->entityId()));
            ImGui::Separator();
            for (const Property& p : object->properties())
            {
                drawProperty(ctx.state.selected, p, ctx.commands);
            }
            if (ImGui::Button("Take Control"))
            {
                ctx.commands.push(EditorCommand::setControlledEntity(ctx.state.selected));
            }
            if (ImGui::Button("Delete"))
            {
                ctx.commands.push(EditorCommand::destroyEntity(ctx.state.selected));
            }
        }

        ImGui::SeparatorText("All Objects");
        drawHierarchy(ctx);

        ImGui::EndTabItem();
    }
}

void UIManager::drawHierarchy(EditorContext &ctx)
{
    for (const EntityInfo& info : ctx.game.getEntities())
    {
        ImGui::PushID(static_cast<int>(info.id));
        if (ImGui::Selectable(info.name.c_str(), ctx.state.selected == info.id))
        {
            ctx.state.selected = info.id;
        }
        ImGui::PopID();
    }

    if (ctx.state.selected != INVALID_ENTITY)
    {
        ImGui::Separator();
        if (ImGui::Button("Delete Selected"))
        {
            ctx.commands.push(EditorCommand::destroyEntity(ctx.state.selected));
        }
    }
}

/*
 * TEMP! Need to create a proper file structure class and behaviour
 */
void UIManager::drawFileTree(EditorContext &ctx)
{
    static FileTreeWidget treeWidget;
    if (ImGui::BeginTabItem("File Tree"))
    {
        treeWidget.draw();
        ImGui::EndTabItem();
    }
}


void UIManager::drawViewport(EditorContext &ctx, SDL_Texture *world_texture)
{
    ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoResize);
    m_viewportSize = ImGui::GetContentRegionAvail();

    // top-left
    const ImVec2 image_pos = ImGui::GetCursorScreenPos();
    ImGui::Image((ImTextureID)(intptr_t)world_texture, m_viewportSize);

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const ImVec2 mouse = ImGui::GetMousePos();
        const glm::vec2 world_pos(mouse.x - image_pos.x, mouse.y - image_pos.y);
        ctx.state.selected = ctx.game.pick(world_pos);

        // temp
        if (ctx.state.sceneBuilding)
        {
            ctx.commands.push(EditorCommand::spawnEntity(ctx.state.defintion, world_pos));
        }
    }

    ImGui::End();
}

void UIManager::render()
{
    ImGui::Render();
}

void UIManager::draw(SDL_Renderer* renderer)
{
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}
