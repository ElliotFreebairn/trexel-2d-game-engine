#include "engine.h"
#include "game_object.h"
#include "player.h"
#include "trexel.h"

#include "imgui_impl_sdlrenderer2.h"
#include "texture_manager.h"
#include "ui_manager.h"
#include <SDL.h>
#include <chrono>
#include <ctime>

Engine::Engine()
    : sdl_context("Trexel", SCREEN_WIDTH, SCREEN_HEIGHT)
    , game_renderer(sdl_context.renderer(), VIEWPORT_WIDTH, VIEWPORT_HEIGHT)
    , ui_manager(sdl_context.window(), sdl_context.renderer())
    , texture_manager()
{
    texture_manager.load(sdl_context.renderer(), "grass_block", "assets/tblock.png");

    // Create trexel game
    Trexel* trexel_game = new Trexel();
    registerGame(static_cast<Game*>(trexel_game));
}

Engine::~Engine()
{
}

void Engine::run()
{
    bool running = true;
    SDL_Event e;

    auto lastFrame = std::chrono::high_resolution_clock::now();
    while(running)
    {
        auto currentFrame = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        running = process_input(e);
        if (!running)
        {
            break;
        }

        update(deltaTime.count());
        render();
    }
}

bool Engine::process_input(SDL_Event &e)
{
    while (SDL_PollEvent(&e))
    {
        ui_manager.process_event(e);
        if (e.type == SDL_QUIT)
            return false;
    }
    return true;
}

void Engine::update(float deltaTime)
{
    auto commands = ui_manager.flush_commands();

    for (const auto& cmd : commands)
    {
        switch (cmd.type)
        {
            case UICommandType::Spawn_Block:
                break;
            case UICommandType::Add_Player:
                /*
                 * Make a game object factory that bridges the ui and creating objects for a game
                 */
                Player* player = new Player(glm::vec2(400.0f, 400.0f), glm::vec2(50.0f, 50.0f), "grass_block");
                game->addObject(static_cast<GameObject*>(player));
                break;
        }
    }

    game->onUpdate(deltaTime);
}

void Engine::render()
{
    // 1. Render the game/scene first
    ImVec2 desired = ui_manager.viewport_size();
    game_renderer.resize(sdl_context.renderer(), (int)desired.x, (int)desired.y);
    game_renderer.render(sdl_context.renderer(), game, texture_manager);

    // 2. Render the UI
    ui_manager.begin_frame();
    ui_manager.build(game_renderer.texture());
    ui_manager.render();

    // 3. Render the composite frame
    SDL_SetRenderDrawColor(sdl_context.renderer(), 30, 30, 30, 255);
    SDL_RenderClear(sdl_context.renderer());
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdl_context.renderer());
    SDL_RenderPresent(sdl_context.renderer());
}

void Engine::registerGame(Game* aGameClass)
{
    game = aGameClass;
}
