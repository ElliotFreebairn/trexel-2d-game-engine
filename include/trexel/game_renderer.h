#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

// Custom includes
#include "game.h"
#include "texture_manager.h"

// SDL Includes
#include <SDL_render.h>

class GameRenderer
{
public:
    GameRenderer(SDL_Renderer* renderer, int width, int height);
    ~GameRenderer();

    // don't allow copying and assignment of game renderer
    GameRenderer(const GameRenderer&) = delete;
    GameRenderer& operator=(const GameRenderer&) = delete;

    void resize(SDL_Renderer* renderer, Game* game, int width, int height);
    void render(SDL_Renderer* renderer, Game* game, const TextureManager& textures); // draws the scene into world_texture_ptr
    SDL_Texture* texture() const { return world_texture_ptr; }
private:
    void create_texture(SDL_Renderer* renderer, int width, int height);

    SDL_Texture* world_texture_ptr = nullptr;
    int width = 0;
    int height = 0;
};

#endif
