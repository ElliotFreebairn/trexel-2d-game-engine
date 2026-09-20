#include "ui_manager.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "mydefines.h"
#include <stdexcept>

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

void UIManager::begin_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    setup_host_window();
}

void UIManager::setup_host_window()
{
    constexpr ImGuiWindowFlags host_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGui::Begin("HostWindow", nullptr, host_flags);
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
    ImGui::End();
}

void UIManager::build(SDL_Texture* world_texture)
{
    ImGui::Begin("Toolbar");

    if (ImGui::Button("Enter Game Mode"))
    {
        m_inGameMode = !m_inGameMode;
    }

    if (ImGui::Button("Toggle Collision Debugging"))
    {
        DEBUG_COLLISION = !DEBUG_COLLISION;
    }

    ImGui::End();

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize);
    m_viewportSize = ImGui::GetContentRegionAvail();

    // Top-left of the image on the screen
    ImVec2 cursor_screen_pos = ImGui::GetCursorScreenPos();
    ImGui::Image((ImTextureID)(intptr_t)world_texture, m_viewportSize);

    // Check interaction inside viewport
    if (ImGui::IsItemHovered())
    {
        // For now calculate viewport every time mouse hovered over viewport
        ImVec2 mouse_pos = ImGui::GetMousePos();
        glm::vec2 local_viewport_pos =
        {
            mouse_pos.x - cursor_screen_pos.x,
            mouse_pos.y - cursor_screen_pos.y
        };

        // Add function that gets loacl viewport pos
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {

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

std::vector<GameAction> UIManager::flush_commands()
{
    std::vector<GameAction> commands = std::move(state.m_pendingCommands);
    state.m_pendingCommands.clear();
    return commands;
}

void UIManager::queueCommand(const GameAction& command)
{
    state.m_pendingCommands.push_back(command);
}
