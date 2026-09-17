#pragma once

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
    SDL_Texture* texture() const { return m_worldTexturePtr; }
private:
    void create_texture(SDL_Renderer* renderer, int width, int height);

    SDL_Texture* m_worldTexturePtr = nullptr;
    int m_width = 0;
    int m_height = 0;
};
