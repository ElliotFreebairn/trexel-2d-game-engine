#ifndef ENGINE_H
#define ENGINE_H

// ImGui
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>

// C+=
#include <stdio.h>

/*
 * Engine Responsibilities:
 *
 */

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr int VIEWPORT_WIDTH = 800;
constexpr int VIEWPORT_HEIGHT = 600;

// Singelton
class Engine {
    public:
        static Engine& getInstance() {
            static Engine engine;
            return engine;
        }

        void run();

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
         * To be moved
         */
        void render_game();
        void render_imgui();
        void build_ui();

        /*
         * Helper functions
         */
        bool init_sdl();
        bool init_imgui();

        // TO BE CONVERTED INTO RESPECTIVE CLASSES/STRUCTURES
        void setup_imgui_frame();
        void setup_imgui_host_window();

        /*
         * Attributes
         */
        //SDL
        SDL_Window* window_ptr;
        SDL_Renderer* renderer_ptr;
        SDL_Texture* world_texture_ptr;
};
#endif
