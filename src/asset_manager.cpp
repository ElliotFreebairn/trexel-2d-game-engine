#include "asset_manager.h"
#include <vector>

void AssetManager::addAsset(std::string id, std::string filePath, AssetType type)
{
    m_assets.push_back(Asset{std::move(id), std::move(filePath), type});
}

const Asset* AssetManager::getAsset(const std::string &id) const
{
    for (const auto& asset : m_assets)
    {
        if (asset.id == id)
        {
            return &asset;
        }
    }
    return nullptr;
}
