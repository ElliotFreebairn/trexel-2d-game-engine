#pragma once

// SDL Includes
#include <SDL.h>

/*
 * SdlContext purpose:
 *  - initialise the SDL context
 *  - own the window and renderer
 *  - supply access to the SDL context to other classes
 */
class SdlContext
{
    public:
        SdlContext(const char* title, int width, int height);
        ~SdlContext();

        SDL_Window*   window()   { return m_windowPtr; }
        SDL_Renderer* renderer() { return m_rendererPtr; }

    private:
        bool initialise();

        SDL_Window* m_windowPtr;
        SDL_Renderer* m_rendererPtr;
};
