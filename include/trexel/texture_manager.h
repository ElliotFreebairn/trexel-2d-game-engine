#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGAER_H

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
        void load(SDL_Renderer* renderer, const std::string& id, const std::string& filePath)
        {
            SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
            if (tex)
            {
                textures[id] = tex;
            }
        }

        SDL_Texture* get(const std::string& id) const
        {
            auto it = textures.find(id);
            return (it != textures.end()) ? it->second : nullptr;
        }

    private:
        std::unordered_map<std::string, SDL_Texture*> textures;
};
#endif
