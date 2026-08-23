#include "engine.h"
#include "game.h"

#include "imgui_impl_sdlrenderer2.h"
#include "texture_manager.h"
#include "ui_manager.h"
#include <SDL.h>

Engine::Engine()
    : sdl_context("Trexel", SCREEN_WIDTH, SCREEN_HEIGHT)
    , game_renderer(sdl_context.renderer(), VIEWPORT_WIDTH, VIEWPORT_HEIGHT)
    , ui_manager(sdl_context.window(), sdl_context.renderer())
    , texture_manager()
{
    texture_manager.load(sdl_context.renderer(), "grass_block", "assets/tblock.png");
}

Engine::~Engine()
{
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
        ui_manager.process_event(e);
        if (e.type == SDL_QUIT)
            return false;
    }
    return true;
}

void Engine::update()
{
    auto commands = ui_manager.flush_commands();

    for (const auto& cmd : commands)
    {
        switch (cmd.type)
        {
            case UICommandType::Spawn_Block:
                game.spawn_block(cmd.viewport_pos);
                break;
        }
    }

    game.update();
}

/*
 * For now lets seperate into SDL and ImGui structs
 * Then port to a more appropiate data structure
 *
 * What would be an appropiate name for SDL part of render. It is the game and scnee
 */

void Engine::render()
{
    /*
     * Testing, REMOVE!!!
     */

    // 1. Render the game/scene first
    ImVec2 desired = ui_manager.viewport_size();
    game_renderer.resize(sdl_context.renderer(), (int)desired.x, (int)desired.y);
    game_renderer.render(sdl_context.renderer(), &game, texture_manager);

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
