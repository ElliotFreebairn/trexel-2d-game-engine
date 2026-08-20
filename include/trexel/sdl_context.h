#ifndef SDL_CONTEXT_H
#define SDL_CONTEXT_H

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

        SDL_Window*   window()   { return window_ptr; }
        SDL_Renderer* renderer() { return renderer_ptr; }

    private:
        bool initialise();

        SDL_Window* window_ptr;
        SDL_Renderer* renderer_ptr;
};

#endif
