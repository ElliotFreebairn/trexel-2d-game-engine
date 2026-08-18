#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <cstdio>
#include <iterator>

Engine::Engine()
{
    if (!init_sdl()) {
        throw "SDL Failed to initialise!";
    }

    if(!init_imgui()) {
        throw "ImGui Failed to initialise!";
    }
}

Engine::~Engine()
{
    // Default ImGui shutdown
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Custom SDL shutdown
    //SDL_DestroyTexture(worldTexture);

    // Default SDL shutdown
    SDL_DestroyRenderer(renderer_ptr);
    SDL_DestroyWindow(window_ptr);
    SDL_Quit();
}

void Engine::run()
{
    bool running = true;
    SDL_Event e;

    while(running)
    {
        running = process_input(e);
        if (!running)
        {
            break;
        }

        update();
        render();
    }
}

bool Engine::process_input(SDL_Event &e)
{
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT)
            return false;
    }
    return true;
}

void Engine::update()
{

}

void Engine::render()
{
    // Split into methods (e.g. SDL & ImGui)
    // SDL Setup
    SDL_SetRenderTarget(renderer_ptr, world_texture_ptr);
    SDL_SetRenderDrawColor(renderer_ptr,
       (Uint8)0,
       (Uint8)0,
       (Uint8)0,
       (Uint8)255
    );
    SDL_RenderClear(renderer_ptr);

    // SDL Game e.g. should be something as render_game()
    SDL_Rect rect = {
        100,
        100,
        100,
        100
    };
    SDL_SetRenderDrawColor(renderer_ptr, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer_ptr, &rect);

    // SDL Reset
    SDL_SetRenderTarget(renderer_ptr, NULL); // back to drawing on the imgui window

    //ImGui Setup
    setup_imgui_frame();

    // ImGui UI e.g. should be something such as build_ui()
    ImGui::Begin("Toolbar");

    ImGui::Button("Add Game");
    ImGui::Text("Game Engine: Trexel");

    ImGui::End();

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize);
    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
    ImVec2 viewport_origin = ImGui::GetCursorScreenPos();
    ImGui::Image((ImTextureID)(intptr_t)world_texture_ptr,
        viewport_size);

    ImGui::End();

    // Important rendering part
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer_ptr, 30, 30, 30, 255);
    SDL_RenderClear(renderer_ptr);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_ptr);
    SDL_RenderPresent(renderer_ptr);
}
/*
 * These should be moved to specific class/file
 *
 * ============================================
 */

// Do we need to return dockspace_id (ImGuiID)?
// This needs to be different and related to frame of
void Engine::setup_imgui_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    setup_imgui_docking();
}

void Engine::setup_imgui_docking()
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

const ImGuiViewport* Engine::setup_imgui_viewport()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);
    ImGui::SetNextWindowViewport(main_viewport->ID);

    // Commented out: currently breaking, figure out why later
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    return main_viewport;
}
// ============================================

bool Engine::init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Could not initialise! SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    // Setup SDL Window
    window_ptr = SDL_CreateWindow("Trexel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window_ptr)
    {
        printf("Window could not initialise! SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    // Setup SDL Renderer
    renderer_ptr = SDL_CreateRenderer(window_ptr, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_ptr)
    {
       printf("Renderer could not initialise! SDL_ERROR: %s\n", SDL_GetError());
       return false;
    }

    world_texture_ptr = SDL_CreateTexture(
        renderer_ptr,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        VIEWPORT_WIDTH, VIEWPORT_WIDTH
    );

    // Successfull
    return true;
}

bool Engine::init_imgui()
{
    if (!window_ptr || !renderer_ptr)
    {
        printf("SDL Window & Renderer must be initialised!");
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(window_ptr, renderer_ptr);
    ImGui_ImplSDLRenderer2_Init(renderer_ptr);
    return true;
}
