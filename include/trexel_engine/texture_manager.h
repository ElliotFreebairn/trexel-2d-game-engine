#pragma once

// Library includes
#include <SDL.h>
#include <SDL_image.h>

// C++ includes
#include <SDL_render.h>
#include <unordered_map>
#include <string>

// implement unique texture std::uniqueptr

class TextureManager
{
    public:
        ~TextureManager()
        {
            for (auto& [id, tex] : m_textures)
            {
                SDL_DestroyTexture(tex);
            }
        }
        TextureManager() = default;
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        void load(SDL_Renderer* renderer, const std::string& id, const std::string& filePath)
        {
            SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
            if (tex)
            {
                m_textures[id] = tex;
            }
        }

        SDL_Texture* get(const std::string& id) const
        {
            auto it = m_textures.find(id);
            return (it != m_textures.end()) ? it->second : nullptr;
        }

    private:
        std::unordered_map<std::string, SDL_Texture*> m_textures;
};
