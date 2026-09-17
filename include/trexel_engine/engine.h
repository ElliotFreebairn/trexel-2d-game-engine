#pragma once

// Custom includes
#include "sdl_context.h"
#include "game_renderer.h"
#include "texture_manager.h"
#include "ui_manager.h"

// SDL
#include <SDL2/SDL.h>

// C++

class Engine {
    public:
        static Engine& getInstance() {
            static Engine engine;
            return engine;
        }

        void run();
        void registerGame(Game* aGameClass);

    private:
        Engine();
        ~Engine();

        /*
         * SDL & ImGui
         */
        bool process_input(SDL_Event &e); // process inputs (SDL Events & ImGui inputs)
        void update(float deltaTime); // update the game state (Physics, Movement etc...)
        void render(); // render SDL graphics and ImGui overlay
        /*
         * Engine owns the SDL & UI contexts
         */
        SdlContext m_sdlContext;
        GameRenderer m_gameRenderer;
        UIManager m_uiManager;
        TextureManager m_textureManager;

        // Should this live here?
        Game* m_game;

        static constexpr int SCREEN_WIDTH = 1280;
        static constexpr int SCREEN_HEIGHT = 720;
        static constexpr int VIEWPORT_WIDTH = 800;
        static constexpr int VIEWPORT_HEIGHT = 600;
};
