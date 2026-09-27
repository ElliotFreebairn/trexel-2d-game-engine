#include "engine.h"
#include "asset.h"
#include "editor_command.h"
#include "editor_state.h"
#include "game.h"
#include "input_state.h"
#include "texture_manager.h"
#include "ui_manager.h"

#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>
#include <chrono>

Engine::Engine(std::unique_ptr<Game> game)
    : m_sdlContext("Trexel", SCREEN_WIDTH, SCREEN_HEIGHT)
    , m_gameRenderer(m_sdlContext.renderer(), VIEWPORT_WIDTH, VIEWPORT_HEIGHT)
    , m_uiManager(m_sdlContext.window(), m_sdlContext.renderer())
    , m_textureManager()
    , m_game(std::move(game))
{
    m_game->setViewport(glm::vec2(VIEWPORT_WIDTH, VIEWPORT_HEIGHT));
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
        const float frameTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
        lastFrame = currentFrame;

        running = process_input(e);
        if (!running)
        {
            break;
        }

        update(frameTime);
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

InputState Engine::sampleInput()
{
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    InputState input;
    if (keys[SDL_SCANCODE_A]) input.move.x -= 1.0f;
    if (keys[SDL_SCANCODE_D]) input.move.x += 1.0f;

    const bool jumpDown = keys[SDL_SCANCODE_SPACE] != 0;
    input.jumpHeld = jumpDown;
    input.jumpPressed = jumpDown && !m_jumpWasDown;
    m_jumpWasDown = jumpDown;

    const bool fireDown = keys[SDL_SCANCODE_F] != 0;
    input.firePressed = fireDown && !m_fireWasDown;
    m_fireWasDown = fireDown;

    return input;
}

void Engine::applyCommands()
{
    const std::vector<EditorCommand> commands = m_commands.flush();
    for (const EditorCommand& command : commands)
    {
        if (command.type == EditorCommandType::SetPlayMode)
        {
            m_editorState.playing = command.playing;
        }
        else
        {
            m_game->applyCommand(command);
        }
    }
}

void Engine::update(float frameTime)
{
    applyCommands();

    // We always want to sample for edge detection
    // If we are not playing, or the UI wants the input, wipe the input state
    InputState input = sampleInput();
    if (!m_editorState.playing || m_uiManager.wantsKeyboard())
    {
        input = InputState{};
    }

    if (!m_editorState.playing)
    {
        m_jumpQueued = false;
        m_fireQueued = false;
        return;
    }

    if (input.jumpPressed) m_jumpQueued = true;
    if (input.firePressed) m_fireQueued = true;

    input.jumpPressed = m_jumpQueued;
    input.firePressed = m_fireQueued;
    m_game->onUpdate(frameTime, input);
    m_jumpQueued = false;
    m_fireQueued = false;
}


void Engine::render()

{
    SDL_Renderer* renderer = m_sdlContext.renderer();

    // 1. Render the game/scene first
    const ImVec2 desired_viewport = m_uiManager.viewport_size();
    const int width = static_cast<int>(desired_viewport.x);
    const int height = static_cast<int>(desired_viewport.y);
    if (m_gameRenderer.resize(renderer, width, height))
    {
        m_game->setViewport(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    }

    // Render options directly manipulate what/how is rendered to the game
    RenderOptions options;
    options.showColliders = m_editorState.showColliders;
    m_gameRenderer.render(renderer, m_game.get(), m_textureManager, options);

    // Build the UI and it get's the read-only game
    // Changing the game via the UI requires applyCommand
    EditorContext ctx{*m_game, m_commands, m_editorState};
    m_uiManager.begin_frame();
    m_uiManager.build(ctx, m_gameRenderer.texture());
    m_uiManager.render();

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    m_uiManager.draw(renderer);
    SDL_RenderPresent(renderer);
}
