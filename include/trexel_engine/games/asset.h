#pragma once
#include <string>

enum class AssetType
{
    Texture
};

struct Asset
{
    std::string id;
    std::string filePath;
    AssetType type = AssetType::Texture;
};
