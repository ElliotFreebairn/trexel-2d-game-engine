#include "ui_manager.h"
#include "editor_command.h"
#include "editor_state.h"
#include "entity.h"
#include "game.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "property.h"

#include <glm/glm.hpp>
#include <stdexcept>

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
    drawToolbar(ctx);
    drawHierarchy(ctx);
    drawInspector(ctx);
    drawViewport(ctx, world_texture);
}

void UIManager::drawToolbar(EditorContext &ctx)
{
    ImGui::Begin("Toolbar");

    if (ImGui::CollapsingHeader("Game Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // The UI only asks. The Engine owns the play mode.
        if (ctx.state.playing)
        {
            if (ImGui::Button("Stop"))
            {
                ctx.commands.push(EditorCommand::setPlayMode(false));
            }
        }
        else
        {
            if (ImGui::Button("Play"))
            {
                ctx.commands.push(EditorCommand::setPlayMode(true));
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Spawn Block"))
        {
            const glm::vec2 centre(m_viewportSize.x * 0.5f, m_viewportSize.y * 0.5f);
            ctx.commands.push(EditorCommand::spawnEntity("block", centre));
        }
    }

    if (ImGui::CollapsingHeader("Debugging", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Collision Boxes", &ctx.state.showColliders);
    }

    ImGui::End();
}

void UIManager::drawHierarchy(EditorContext &ctx)
{
    ImGui::Begin("Hierachy");

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

    ImGui::End();
}

void UIManager::drawInspector(EditorContext &ctx)
{
    ImGui::Begin("Inspector");

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

    ImGui::End();
}

void UIManager::drawViewport(EditorContext &ctx, SDL_Texture *world_texture)
{
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize);
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
