#include "file_tree_widget.h"
#include <imgui.h>

/*
 * Completely angostic of tab item, etc... can live anywhere
 */
void FileTreeWidget::draw()
{
    // TEMP - Start at the project root directy (Decide what should this default to)
    std::string path = fs::current_path();

    drawNode(path);
}

void FileTreeWidget::drawNode(const fs::path &path)
{
    /*
    * We want to order the entries drawn top to bottom via directory to file and then via the alphabet
    */
    std::vector<std::filesystem::directory_entry> directories = sortByDirectoryType(path);
    for (const auto& entry : directories)
    {
        std::string filename = entry.path().filename().string();

        if (entry.is_directory())
        {
            if (ImGui::TreeNode(filename.c_str()))
            {
                drawNode(entry.path());
                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::Text("%s", filename.c_str());
        }
    }
}

std::vector<std::filesystem::directory_entry> FileTreeWidget::sortByDirectoryType(std::string path) const
{
    std::vector<fs::directory_entry> directories;
    std::vector<fs::directory_entry> files;

    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.is_directory()) directories.push_back(entry);
        else files.push_back(entry);
    }

    directories.insert_range(directories.end(), files);
    return directories;
}
