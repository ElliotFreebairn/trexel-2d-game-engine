#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <cstdint>
#include <cstdlib>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// The "world" rendered at its own resolution
const int VIEWPORT_WIDTH = 800;
const int VIEWPORT_HEIGHT = 600;

// Start up SDL and creates window_ptr
bool init(SDL_Window** window, SDL_Renderer** renderer)
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Could not initalise! SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    // Create a window_ptr
    *window = SDL_CreateWindow("Trexel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window)
    {
        printf("window_ptr could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        SDL_DestroyWindow(*window);
    }

    // init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(*window, *renderer);
    ImGui_ImplSDLRenderer2_Init(*renderer);
    return true;
}

// Frees media and shuts down SDL
void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* worldTexture)
{
    // Shutdown imgui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(worldTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

const ImGuiViewport* setup_main_viewport() {
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);
    ImGui::SetNextWindowViewport(main_viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    return main_viewport;
}

ImGuiID setup_docking() {
    constexpr ImGuiWindowFlags host_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGui::Begin("HostWindow", nullptr, host_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    ImGui::End();

    return dockspace_id;
}

// Place imgui context within SDL window_ptr, not surface
int main(int argc, char *argv[]) {
    SDL_Window* window_ptr = NULL;
    SDL_Renderer* renderer_ptr = NULL;

    if (init(&window_ptr, &renderer_ptr) == false) {
        printf("SDL initalisaiton failed! SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }

    // The off-screen texture the "world" gets drawn to
    SDL_Texture* worldTexture = SDL_CreateTexture(
        renderer_ptr,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        VIEWPORT_WIDTH, VIEWPORT_HEIGHT
    );

    // Keep the window_ptr open
    SDL_Event e;
    bool quit = false;

    while(quit == false)
    {
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
                quit = true;
        }

        // ========================= SDL / Game logic ================================/
        // SETUP
        SDL_SetRenderTarget(renderer_ptr, worldTexture);
        SDL_SetRenderDrawColor(renderer_ptr,
            (Uint8)0,
            (Uint8)0,
            (Uint8)0,
            255
        );
        SDL_RenderClear(renderer_ptr);

        // GAME
        SDL_Rect square = { 350, 250, 100, 100 };
        SDL_SetRenderDrawColor(renderer_ptr, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer_ptr, &square);

        // RESET
        SDL_SetRenderTarget(renderer_ptr, NULL); // back to drawing on the window

        // ========================= ImGui  ================================/
        // SETUP
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        const ImGuiViewport* main_viewport = setup_main_viewport();
        ImGuiID dockingspace_id = setup_docking();

        // UI BUILDING
        ImGui::Begin("Toolbar");
        if (ImGui::Button("Change rect colour")) {
            printf("Button clicked");
        }
        ImGui::End();

        ImGui::Begin("Viewport", nullptr);
        ImGui::Image((ImTextureID)(intptr_t)worldTexture,
            ImVec2((float)VIEWPORT_WIDTH, (float)VIEWPORT_HEIGHT));
        ImGui::End();

        // ========================= Render SDL & ImGui  ================================/
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer_ptr, 30, 30, 30, 255);
        SDL_RenderClear(renderer_ptr);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_ptr);
        SDL_RenderPresent(renderer_ptr);
    }

    close(window_ptr, renderer_ptr, worldTexture);

    return 0;
}
