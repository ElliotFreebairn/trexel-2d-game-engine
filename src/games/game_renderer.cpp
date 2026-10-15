#include "game_renderer.h"
#include "IRenderable.h"
#include "common.h"
#include "game.h"
#include "mydefines.h"

#include <SDL_blendmode.h>
#include <SDL_image.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <format>
#include <stdexcept>

GameRenderer::GameRenderer(SDL_Renderer* renderer, int width, int height)
{
    create_texture(renderer, width, height);
}

GameRenderer::~GameRenderer()
{
    if (m_worldTexturePtr)
    {
        SDL_DestroyTexture(m_worldTexturePtr);
    }
}

/*
 * Game renderer should take in a game
 * That game, for now, is built programatically
 */
void GameRenderer::render(SDL_Renderer* renderer, Game* game, const TextureManager& textures, const RenderOptions& options)
{
    SDL_SetRenderTarget(renderer, m_worldTexturePtr);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    std::string backgroundId = game->getBackground();
    if (!backgroundId.empty())
    {
        SDL_Texture* bgTexture = textures.get(backgroundId);
        if (bgTexture)
        {
            SDL_Rect destRect = {0, 0, m_width, m_height};
            SDL_RenderCopy(renderer, bgTexture, nullptr, &destRect);
        }
    }

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

    //#define DEBUG_DRAW_COLLIDERS
    if (options.showColliders)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (const IRenderable* cmd : renderables)
        {
            if (!cmd->hasCollider()) continue;
            AABB b = cmd->getBounds();
            SDL_Rect r{
                static_cast<int>(b.min.x),
                static_cast<int>(b.min.y),
                static_cast<int>(b.max.x - b.min.x),
                static_cast<int>(b.max.y - b.min.y)
            };
            SDL_RenderDrawRect(renderer, &r);
        }
    }
    SDL_SetRenderTarget(renderer, nullptr); // restore default target
}

bool GameRenderer::resize(SDL_Renderer* renderer,  int width, int height)
{
    if (width <= 0 || height <= 0) return false;
    if(width == this->m_width && height == this->m_height) return false;

    create_texture(renderer, width, height);
    return true;
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

    if (m_worldTexturePtr) SDL_DestroyTexture(m_worldTexturePtr);
    m_worldTexturePtr = new_texture;
    this->m_width = width;
    this->m_height = height;
}
