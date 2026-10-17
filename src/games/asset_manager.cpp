#include "asset_manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "asset.h"
#include "nlohmann/json.hpp"
#include "asset_json.h"

using json = nlohmann::json;

void AssetManager::loadManifest(const std::filesystem::path &path)
{
    std::ifstream file(path);
    if (!file) return;

    const auto manifest = nlohmann::json::parse(file, nullptr, false);
    if (manifest.is_discarded() || !manifest.contains("assets")) return;

    m_manfiestPath = path;
    m_assetRoot = path.parent_path() / "assets";
    for (const auto& entry : manifest["assets"])
    {
        try
        {
            addAsset(entry.get<Asset>());
        }
        catch (const nlohmann::json::exception& e)
        {
            std::cerr << "Bad Asset: " << e.what() << "\n";
        }
    }
}

void AssetManager::saveManifest(const std::filesystem::path &path) const
{
    nlohmann::json manifest;
    manifest["assets"] = m_assets;
    std::ofstream(path) << manifest.dump(4);
}

bool AssetManager::addAsset(Asset asset)
{
    if (getAsset(asset.id)) return false;
    m_assets.push_back(std::move(asset));
    return true;
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

std::filesystem::path AssetManager::resolvePath(const Asset &a) const
{
    return m_assetRoot / a.filePath;
}
