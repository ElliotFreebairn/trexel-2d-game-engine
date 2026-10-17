#pragma once

// Custom includes
#include "asset.h"
#include "nlohmann/json.hpp"

// C++ includes
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>


class AssetManager
{
public:
    void loadManifest(const std::filesystem::path& path);
    void saveManifest(const std::filesystem::path& path) const;

    bool addAsset(Asset asset);

    const std::vector<Asset>& getAssets() const { return m_assets; }
    const Asset* getAsset(const std::string& id) const;

    std::filesystem::path resolvePath(const Asset& a) const;

private:
    std::vector<Asset> m_assets;

    std::filesystem::path m_manfiestPath;
    std::filesystem::path m_assetRoot;
};
