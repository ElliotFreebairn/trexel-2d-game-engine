#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SDL.h>
#include "imgui.h"
// Potentially UI Manager could be an abstract class
//  ImGuiManager would specialised
class UIManager
{
public:
    UIManager(SDL_Window* window, SDL_Renderer* renderer);
    ~UIManager();

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    void process_event(const SDL_Event& e);

    void begin_frame();
    void render();
    void build(SDL_Texture* world_texture);
    void draw(SDL_Renderer* renderer);

    ImVec2 viewport_size() const { return viewport_size_; }

private:
    void setup_host_window();

    ImVec2 viewport_size_ = ImVec2(800.0f, 600.0f);

};
#endif
