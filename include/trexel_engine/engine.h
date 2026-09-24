#pragma once

// Custom includes
#include "editor_command.h"
#include "editor_state.h"
#include "input_state.h"
#include "sdl_context.h"
#include "game_renderer.h"
#include "texture_manager.h"
#include "ui_manager.h"

// SDL
#include <SDL2/SDL.h>

// C++

class Engine {
    public:
        explicit Engine(std::unique_ptr<Game> game);
        ~Engine() = default;

        void run();

    private:
        /*
         * SDL & ImGui
         */
        bool process_input(SDL_Event &e); // process inputs (SDL Events & ImGui inputs)
        void update(float deltaTime); // update the game state (Physics, Movement etc...)
        void render(); // render SDL graphics and ImGui overlay

        // Simulation
        void applyCommands();
        InputState sampleInput();

        /*
         * Engine owns the SDL & UI contexts
         */
        SdlContext m_sdlContext;
        GameRenderer m_gameRenderer;
        UIManager m_uiManager;
        TextureManager m_textureManager;
        std::unique_ptr<Game> m_game;

        // Editor
        CommandQueue m_commands;
        EditorState m_editorState;

        // Input
        float m_accumulator = 0.0f;
        bool m_jumpWasDown = false;
        bool m_jumpQueued = false;

        static constexpr int SCREEN_WIDTH = 1280;
        static constexpr int SCREEN_HEIGHT = 720;
        static constexpr int VIEWPORT_WIDTH = 800;
        static constexpr int VIEWPORT_HEIGHT = 600;
};
