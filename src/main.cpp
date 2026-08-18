// Custom includes
#include "engine.h"

// C++ includes
#include <stdio.h>


// Function that places a block inside the SDL frame based on the mouse pos
// This requires a function that can correctly determine the mouse pos inside the SDL frame inside ImGui frame
//
struct HoverInfo {
    const char* window_name = "none";
    ImVec2 relative_pos = ImVec2(0, 0);
};

ImVec2 GetMousePosRelativeTo(ImVec2 origin) {
    return ImVec2(
        ImGui::GetMousePos().x - origin.x,
        ImGui::GetMousePos().y - origin.y
    );
}

// Place imgui context within SDL window_ptr, not surface
int main(int argc, char *argv[]) {
    Engine& engine = Engine::getInstance();

    engine.run();

    return 0;
}
