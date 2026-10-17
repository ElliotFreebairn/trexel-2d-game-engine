#include "asset_manager.h"
#include <expected>
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

void AssetManager::saveManifest() const
{
    nlohmann::json manifest;
    manifest["assets"] = m_assets;
    std::ofstream(m_manfiestPath) << manifest.dump(4);
}

std::expected<void, AssetError> AssetManager::addAsset(Asset asset)
{
    if (auto result = validate(asset); !result) // if-with initiaiser (c++17)
        return result;

    m_assets.push_back(std::move(asset));
    return {};
}

std::expected<void, AssetError> AssetManager::validate(const Asset &asset) const
{
    if (asset.id.empty()) return std::unexpected(AssetError::EmptyID);
    if (getAsset(asset.id)) return std::unexpected(AssetError::DuplicateID);
    if (asset.filePath.empty()) return std::unexpected(AssetError::EmptyPath);
    if (!std::filesystem::exists(resolvePath(asset))) return std::unexpected(AssetError::FileNotFound);

    return {}; // success, an expected <void>
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

std::string_view describe(AssetError error)
{
    switch (error)
    {
        case AssetError::EmptyID:      return "Asset ID is empty";
        case AssetError::DuplicateID:  return "An asset with this ID already exists";
        case AssetError::EmptyPath:    return "Asset file path is empty";
        case AssetError::FileNotFound: return "Asset file not found";
    }
    return "Unknown asset error";
}
