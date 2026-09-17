#include "engine.h"
#include "asset.h"
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

Engine::Engine(std::unique_ptr<Game> game)
    : m_sdlContext("Trexel", SCREEN_WIDTH, SCREEN_HEIGHT)
    , m_gameRenderer(m_sdlContext.renderer(), VIEWPORT_WIDTH, VIEWPORT_HEIGHT)
    , m_uiManager(m_sdlContext.window(), m_sdlContext.renderer())
    , m_textureManager()
    , m_game(std::move(game))
{
    m_game->init();

    for (const Asset& asset : m_game->getAssetManager().getAssets())
    {
        if (asset.type == AssetType::Texture)
        {
            m_textureManager.load(m_sdlContext.renderer(), asset.id, asset.filePath);
        }
    }
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
        m_uiManager.process_event(e);
        if (e.type == SDL_QUIT)
            return false;
    }
    return true;
}

void Engine::update(float deltaTime)
{
    auto commands = m_uiManager.flush_commands();

    for (const auto& cmd : commands)
    {
        m_game->handleAction(cmd);
    }

    /*
     * CREATE AN GAME INPUT MANAGER THAT DEALS WITH THIS
     */
    if (m_uiManager.getGameMode())
    {
        m_game->onUpdate(deltaTime);

        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        glm::vec2 dir{0.0f};
        bool jumping = false;

        if (keys[SDL_SCANCODE_W]) dir.y -= 1.0f;
        if (keys[SDL_SCANCODE_S]) dir.y += 1.0f;
        if (keys[SDL_SCANCODE_A]) dir.x -= 1.0f;
        if (keys[SDL_SCANCODE_D]) dir.x += 1.0f;

        if (dir != glm::vec2{0.0f})
        {
            m_game->handleAction(GameAction{
                .type = GameActionType::MovePlayer,
                .direction = dir * deltaTime
            });
        }

        bool jumpKeyIsDown = keys[SDL_SCANCODE_SPACE];
        if (jumpKeyIsDown && !jumpKeyWasDown)
        {
            m_game->handleAction(GameAction{.type = GameActionType::Jump});
        }
    }
}

void Engine::render()
{
    // 1. Render the game/scene first
    ImVec2 desired = m_uiManager.viewport_size();
    m_gameRenderer.resize(m_sdlContext.renderer(), m_game.get(), (int)desired.x, (int)desired.y);
    m_gameRenderer.render(m_sdlContext.renderer(), m_game.get(), m_textureManager);

    // 2. Render the UI
    m_uiManager.begin_frame();
    m_uiManager.build(m_gameRenderer.texture());
    m_uiManager.render();

    // 3. Render the composite frame
    SDL_SetRenderDrawColor(m_sdlContext.renderer(), 30, 30, 30, 255);
    SDL_RenderClear(m_sdlContext.renderer());
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_sdlContext.renderer());
    SDL_RenderPresent(m_sdlContext.renderer());
}
