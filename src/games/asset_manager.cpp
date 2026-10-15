#include "asset_manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "asset.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void AssetManager::loadManifest(const std::filesystem::path &path)
{
    // check if the file exists
    if (path.empty() || !path.has_filename()) return;

    // load the file
    std::fstream file(path);

    // can we open the manifest located in storage
    if (!file.is_open()) return;

    // parse the file into a json blob
    json manifestJson = json::parse(file);

    if (!manifestJson.contains("assets"))
    {
        std::cout << "AssetManager Error! manifest doesn't contain assets\n";
        return;
    }

    for (const auto asset : manifestJson["assets"])
    {
        std::cout << asset << "\n";

        // get the id
        std::string id = asset["id"];

        // get the texture type
        std::string type = asset["type"];
        // for now we are going to assume is Asset::Texture
        AssetType aType = AssetType::Texture;

        // get the path relative to assets/
        // sort out this manual relative path issue !!!
        std::filesystem::path path = std::filesystem::path("data/assets") / asset["path"];

        std::cout << path << "\n";

        if (id.empty() || type.empty() || path.empty())
        {
            std::cout << "AssetManager Error! asset is mising a parameter\n";
            continue;
        }

        m_assets.push_back(Asset{id, path, aType});
    }
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
