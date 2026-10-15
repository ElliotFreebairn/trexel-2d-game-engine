#include "sdl_context.h"

// C++ includes
#include <SDL_error.h>
#include <SDL_image.h>
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
    m_windowPtr = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!m_windowPtr)
    {
        std::string error_message = std::format("Window could not initialise! SDL_ERROR: {}\n", SDL_GetError());
        throw std::runtime_error(error_message);
    }

    // Setup SDL Renderer
    m_rendererPtr = SDL_CreateRenderer(m_windowPtr, -1, SDL_RENDERER_ACCELERATED);
    if (!m_rendererPtr)
    {
        SDL_DestroyWindow(m_windowPtr);
        SDL_Quit();

        std::string error_message = std::format("Renderer could not initialise! SDL_ERROR: {}\n", SDL_GetError());
        throw std::runtime_error(error_message);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::string error_message = std::format("IMG_INIT failed: {}", IMG_GetError());
        throw std::runtime_error(error_message);
    }
}

SdlContext::~SdlContext()
{
    SDL_DestroyRenderer(m_rendererPtr);
    SDL_DestroyWindow(m_windowPtr);
    SDL_Quit();
}
