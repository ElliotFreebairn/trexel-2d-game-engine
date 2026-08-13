#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Start up SDL and creates window
bool init(SDL_Window** window, SDL_Surface** surface)
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Could not initalise! SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    // Create a window
    *window = SDL_CreateWindow("Trexel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    *surface = SDL_GetWindowSurface(*window);

    return true;
}

// Frees media and shuts down SDL
void close(SDL_Window* window)
{
    // Destroy the window
    SDL_DestroyWindow(window);
    // Quit the SDL Subsystems
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    if (init(&window, &screenSurface) == false) {
        printf("SDL initalisaiton failed! SDL_Error: %s\n", SDL_GetError());
    }

    // Fill the surface white
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(window);

    // Keep the window open
    SDL_Event e;
    bool quit = false;
    while(quit == false)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }

    return 0;
}
