#pragma once

// Custom includes
#include "asset.h"

// C++ includes
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

class AssetManager
{
public:
    void loadManifest(const std::filesystem::path& path);

    const std::vector<Asset>& getAssets() const { return m_assets; }
    const Asset* getAsset(const std::string& id) const;

private:
    std::vector<Asset> m_assets;
};
