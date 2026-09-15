#include "engine.h"
#include "game_object.h"
#include "player.h"
#include "trexel.h"
#include "game_actions.h"

#include "imgui_impl_sdlrenderer2.h"
#include "texture_manager.h"
#include "ui_manager.h"
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <chrono>
#include <ctime>

static bool jumpKeyWasDown = false;

Engine::Engine()
    : sdl_context("Trexel", SCREEN_WIDTH, SCREEN_HEIGHT)
    , game_renderer(sdl_context.renderer(), VIEWPORT_WIDTH, VIEWPORT_HEIGHT)
    , ui_manager(sdl_context.window(), sdl_context.renderer())
    , texture_manager()
{
    // Really need a game asset manager
    texture_manager.load(sdl_context.renderer(), "grass_block", "assets/tblock.png");
    texture_manager.load(sdl_context.renderer(), "player", "assets/player.png");
    texture_manager.load(sdl_context.renderer(), "player_sheet", "assets/player_sheet.png");
    texture_manager.load(sdl_context.renderer(), "idle_player", "assets/Idle.png");
    texture_manager.load(sdl_context.renderer(), "walking_player", "assets/vampire_walking.png");
    texture_manager.load(sdl_context.renderer(), "jumping_player", "assets/Jump.png");

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
        game->handleAction(cmd);
    }

    /*
     * CREATE AN GAME INPUT MANAGER THAT DEALS WITH THIS
     */
    if (ui_manager.getGameMode())
    {
        game->onUpdate(deltaTime);


        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        glm::vec2 dir{0.0f};
        bool jumping = false;

        if (keys[SDL_SCANCODE_W]) dir.y -= 1.0f;
        if (keys[SDL_SCANCODE_S]) dir.y += 1.0f;
        if (keys[SDL_SCANCODE_A]) dir.x -= 1.0f;
        if (keys[SDL_SCANCODE_D]) dir.x += 1.0f;

        if (dir != glm::vec2{0.0f})
        {
            game->handleAction(GameAction{
                .type = GameActionType::MovePlayer,
                .direction = dir * deltaTime
            });
        }

        bool jumpKeyIsDown = keys[SDL_SCANCODE_SPACE];
        if (jumpKeyIsDown && !jumpKeyWasDown)
        {
            game->handleAction(GameAction{.type = GameActionType::Jump});
        }
    }
}

void Engine::render()
{
    // 1. Render the game/scene first
    ImVec2 desired = ui_manager.viewport_size();
    game_renderer.resize(sdl_context.renderer(), game, (int)desired.x, (int)desired.y);
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
