#ifndef ENGINE_H
#define ENGINE_H

// Custom includes
#include "sdl_context.h"
#include "game_renderer.h"
#include "texture_manager.h"
#include "ui_manager.h"

// ImGui
#include "imgui.h"
// SDL
#include <SDL2/SDL.h>
// C+=
#include <stdio.h>

/*
 * Engine Responsibilities:
 *
 */
// Singelton
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
        void update(); // update the game state (Physics, Movement etc...)
        void render(); // render SDL graphics and ImGui overlay
        /*
         * Engine owns the SDL & UI contexts
         */
        SdlContext sdl_context;
        GameRenderer game_renderer;
        UIManager ui_manager;
        TextureManager texture_manager;

        // Should this live here?
        Game* game;

        static constexpr int SCREEN_WIDTH = 1280;
        static constexpr int SCREEN_HEIGHT = 720;
        static constexpr int VIEWPORT_WIDTH = 800;
        static constexpr int VIEWPORT_HEIGHT = 600;
};
#endif
