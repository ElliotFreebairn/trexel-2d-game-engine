#include "game_renderer.h"
#include "game.h"
#include "game_object.h"

#include <SDL_blendmode.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <format>
#include <memory>
#include <numbers>
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

/*
 * Game renderer should take in a game
 * That game, for now, is built programatically
 */
void GameRenderer::render(SDL_Renderer* renderer, Game* game, const TextureManager& textures)
{
    SDL_SetRenderTarget(renderer, world_texture_ptr);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // --- scene contents go here ---
    // it should only deal with IRenderable objects
    const std::vector<std::unique_ptr<GameObject>>& game_objects = game->get_entities();

    for (const std::unique_ptr<GameObject>& obj : game_objects)
    {
        SDL_Rect rect = {
            static_cast<int>(obj->get_position().x),
            static_cast<int>(obj->get_position().y),
            static_cast<int>(obj->get_size().x),
            static_cast<int>(obj->get_size().y)
        };
        SDL_Texture* tex = textures.get(obj->get_texture_id());
        if (tex)
        {
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
            SDL_RenderCopy(renderer, tex, nullptr, &rect);
        }
        else
        {
            // falback renderer if tex is missing
            glm::vec3 colour = obj->get_colour().value_or(glm::vec3(0.0f, 0.0f, 0.0f));
            SDL_SetRenderDrawColor(renderer, (Uint8)colour.x, (Uint8)colour.y, (Uint8)colour.z, 255); // Magenta error box
            SDL_RenderFillRect(renderer, &rect);
        }
    }

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
    SDL_SetTextureBlendMode(new_texture, SDL_BLENDMODE_BLEND);

    if (world_texture_ptr) SDL_DestroyTexture(world_texture_ptr);
    world_texture_ptr = new_texture;
    this->width = width;
    this->height = height;
}
