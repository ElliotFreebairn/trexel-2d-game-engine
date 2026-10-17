#pragma once

// Custom includes
#include "asset.h"
#include "nlohmann/json.hpp"

// C++ includes
#include <string_view>
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>
#include <expected>

enum class AssetError { EmptyID, DuplicateID, EmptyPath, FileNotFound };

std::string_view describe(AssetError error);

class AssetManager
{
public:
    void loadManifest(const std::filesystem::path& path);
    void saveManifest() const;

    std::expected<void, AssetError> addAsset(Asset asset);

    const std::vector<Asset>& getAssets() const { return m_assets; }
    const Asset* getAsset(const std::string& id) const;

    std::filesystem::path resolvePath(const Asset& a) const;

    // read-only
    std::expected<void, AssetError> validate(const Asset& asset) const;

private:
    std::vector<Asset> m_assets;

    std::filesystem::path m_manfiestPath;
    std::filesystem::path m_assetRoot;
};
