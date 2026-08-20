#include "game_renderer.h"

#include <SDL_pixels.h>
#include <SDL_render.h>
#include <format>
#include <stdexcept>

GameRenderer::GameRenderer(SDL_Renderer* renderer, int width, int height)
{
   world_texture_ptr = SDL_CreateTexture(
       renderer,
       SDL_PIXELFORMAT_RGBA8888,
       SDL_TEXTUREACCESS_TARGET,
       width, height
   ) ;
   if (!world_texture_ptr)
   {
       std::string error_message = std::format("SDL_CreateTexture failed, SDL_ERROR: {}", SDL_GetError());
       throw std::runtime_error(error_message);
   }
}

GameRenderer::~GameRenderer()
{
    if (world_texture_ptr)
    {
        SDL_DestroyTexture(world_texture_ptr);
    }
}

void GameRenderer::render(SDL_Renderer* renderer)
{
    SDL_SetRenderTarget(renderer, world_texture_ptr);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // --- scene contents go here ---
    SDL_Rect rect = { 100, 100, 100, 100 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    // -------------------------------

    SDL_SetRenderTarget(renderer, nullptr); // restore default target
}

void GameRenderer::resize(SDL_Renderer* renderer, int width, int height)
{
    if (width <= 0 || height <= 0) return;
    if(width == this->width && height == this->height) return;
    create_texture(renderer, width, height);
}

void GameRenderer::create_texture(SDL_Renderer* renderer, int width, int height)
{
    SDL_Texture* new_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height
    );
    if (!new_texture)
    {
        throw std::runtime_error("Creating new texture has failed!");
    }
    if (world_texture_ptr) SDL_DestroyTexture(world_texture_ptr);
    world_texture_ptr = new_texture;
    this->width = width;
    this->height = height;
}
