#include "sdl_context.h"

// C++ includes
#include <SDL_error.h>
#include <stdexcept>
#include <format>

SdlContext::SdlContext(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::string error_message = std::format("SDL Could not initialise! SDL_ERROR: {}\n", SDL_GetError());
        throw std::runtime_error(error_message);
    }

    // Setup SDL Window
    window_ptr = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window_ptr)
    {
        std::string error_message = std::format("Window could not initialise! SDL_ERROR: {}\n", SDL_GetError());
        throw std::runtime_error(error_message);
    }

    // Setup SDL Renderer
    renderer_ptr = SDL_CreateRenderer(window_ptr, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_ptr)
    {
        SDL_DestroyWindow(window_ptr);
        SDL_Quit();

        std::string error_message = std::format("Renderer could not initialise! SDL_ERROR: {}\n", SDL_GetError());
        throw std::runtime_error(error_message);
    }
}

SdlContext::~SdlContext()
{
    SDL_DestroyRenderer(renderer_ptr);
    SDL_DestroyWindow(window_ptr);
    SDL_Quit();
}
