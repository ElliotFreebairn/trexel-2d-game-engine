#pragma once

// Library includes
#include <SDL.h>
#include <SDL_image.h>

// C++ includes
#include <SDL_render.h>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <functional>
#include <iostream>

// implement unique texture std::uniqueptr

class TextureManager
{
    public:
        // given an id, where is the file? TextureManager doesn't need to know about AssetManager
        using Resolver = std::function<std::optional<std::filesystem::path>(const std::string& id)>;

        TextureManager(SDL_Renderer* renderer, Resolver resolver) : m_renderer(renderer), m_resolver(resolver) {}

        SDL_Texture* get(const std::string& id) const
        {
            if (auto it = m_cache.find(id); it != m_cache.end())
            {
                return it->second.get();
            }

            auto [it, inserted] = m_cache.emplace(id, load(id));
            return it->second.get();
        }

        void invalidate(const std::string& id) { m_cache.erase(id); }

    private:
        struct SdlTextureDeleter
        {
            void operator()(SDL_Texture* t) const { SDL_DestroyTexture(t); }
        };
        using TexturePtr = std::unique_ptr<SDL_Texture, SdlTextureDeleter>;

        TexturePtr load(const std::string& id) const
        {
            const auto path = m_resolver(id);
            if (!path) return nullptr;

            TexturePtr tex{IMG_LoadTexture(m_renderer, path->string().c_str())};
            if (!tex) std::cerr << "TextureManager: failed to load " << *path << ": " << IMG_GetError() << "\n";
            return tex;
        }

        SDL_Renderer* m_renderer;
        Resolver m_resolver;
        mutable std::unordered_map<std::string, TexturePtr> m_cache;
};
