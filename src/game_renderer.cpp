#include "game_renderer.h"
#include "IRenderable.h"
#include "common.h"
#include "game.h"

#include <SDL_blendmode.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <format>
#include <memory>
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
    const std::vector<const IRenderable*> renderables = game->getRenderables();

    for (const IRenderable* cmd : renderables)
    {
        SDL_Rect rect = {
            static_cast<int>(cmd->getPosition().x),
            static_cast<int>(cmd->getPosition().y),
            static_cast<int>(cmd->getSize().x),
            static_cast<int>(cmd->getSize().y)
        };
        SDL_Texture* tex = textures.get(cmd->getTextureID());
        if (tex)
        {
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

            auto srcRectOpt = cmd->getSourceRect();
            SDL_Rect sdlSrc;
            const SDL_Rect* srcRect = nullptr;
            if (srcRectOpt)
            {
                sdlSrc = SDL_Rect {srcRectOpt->x, srcRectOpt->y, srcRectOpt->w, srcRectOpt->h};
                srcRect = &sdlSrc;
            }
            SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;
            Flip f = cmd->getFlip();
            switch (f)
            {
                case Flip::NONE:
                    sdlFlip = SDL_FLIP_NONE;
                    break;
                case Flip::HORIZONTAL:
                    sdlFlip = SDL_FLIP_HORIZONTAL;
                    break;
                case Flip::VERTICAL:
                    sdlFlip = SDL_FLIP_VERTICAL;
                    break;
            }
            SDL_RenderCopyEx(renderer, tex, srcRect, &rect, cmd->getAngle(), NULL, sdlFlip);
        }
        else
        {
            // falback renderer if tex is missing
            glm::vec3 colour = cmd->getColour().value_or(glm::vec3(0.0f, 0.0f, 0.0f));
            SDL_SetRenderDrawColor(renderer, (Uint8)colour.x, (Uint8)colour.y, (Uint8)colour.z, 255); // Magenta error box
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr); // restore default target
}

void GameRenderer::resize(SDL_Renderer* renderer, Game* game, int width, int height)
{
    if (width <= 0 || height <= 0) return;
    if(width == this->width && height == this->height) return;

    game->setViewport({width, height});

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
