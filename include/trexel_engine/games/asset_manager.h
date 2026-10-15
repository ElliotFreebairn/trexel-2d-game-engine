#pragma once

// Custom includes
#include "asset.h"

// C++ includes
#include <vector>
#include <tuple>
#include <string>

class AssetManager
{
public:
    const std::vector<Asset>& getAssets() const { return m_assets; }
    const Asset* getAsset(const std::string& id) const;

    void addAsset(std::string id, std::string filePath, AssetType type);
private:
    std::vector<Asset> m_assets;
};
