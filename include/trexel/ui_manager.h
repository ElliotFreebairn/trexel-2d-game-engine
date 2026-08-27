#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SDL.h>
#include "glm/glm.hpp"
#include "imgui.h"

// move later
enum class UICommandType
{
    Spawn_Block,
    Add_Player
};

struct UICommand
{
    UICommandType type;
    glm::vec2 viewport_pos; // click relative to viewport (0, 0 to width, height)
};

struct UIState
{
    std::vector<UICommand> pending_commands;
};

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

    void queueCommand(const UICommand& command);
    std::vector<UICommand> flush_commands();

private:
    UIState state;

    void setup_host_window();

    ImVec2 viewport_size_ = ImVec2(800.0f, 600.0f);

};
#endif
