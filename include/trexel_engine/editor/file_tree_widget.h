#pragma once

#include "IWidget.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class FileTreeWidget : public IWidget
{
public:
    void draw() override;

private:
    fs::path m_rootPath;
    fs::path m_currentPath;

    void drawNode(const fs::path& path);
    std::vector<std::filesystem::directory_entry> sortByDirectoryType(std::string path) const;
};
