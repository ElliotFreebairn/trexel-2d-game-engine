#include "ui_manager.h"
#include "editor_command.h"
#include "editor_state.h"
#include "entity.h"
#include "game.h"
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
        ImGui::LabelText(label, "%s", s->c_str());
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

        drawInspector(ctx);

        drawHierarchy(ctx);

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

        //ImGui::SameLine();
        //if (ImGui::Button("Spawn Block"))
        //{
        //    const glm::vec2 centre(m_viewportSize.x * 0.5f, m_viewportSize.y * 0.5f);
        //    ctx.commands.push(EditorCommand::spawnEntity("block", centre));
        //}

        ImGui::Separator();
        ImGui::TextDisabled("VISUALS");
        ImGui::Checkbox("Collision Boxes", &ctx.state.showColliders);

        ImGui::Separator();
        ImGui::TextDisabled("WORLD BUILDING");

        ImGui::Separator();
        ImGui::TextDisabled("Asset Management");
        drawAssetManager(ctx);

        ImGui::EndTabItem();
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


void UIManager::drawHierarchy(EditorContext &ctx)
{
    if (ImGui::BeginTabItem("Hiearchy"))
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
        ImGui::EndTabItem();}
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
        }
        ImGui::EndTabItem();
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
