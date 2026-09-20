#pragma once

#include <SDL.h>
#include "glm/glm.hpp"
#include "imgui.h"

#include "game_actions.h"

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

    ImVec2 viewport_size() const { return m_viewportSize; }

    void queueCommand(const GameAction& command);
    std::vector<GameAction> flush_commands();

    bool getGameMode() { return m_inGameMode; }

private:
    struct {
        std::vector<GameAction> m_pendingCommands;
    } state;

    void setup_host_window();

    ImVec2 m_viewportSize = ImVec2(800.0f, 600.0f);
    bool m_inGameMode = false;
};
