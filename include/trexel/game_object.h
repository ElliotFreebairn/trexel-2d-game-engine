#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


// Library includes
#include <SDL_render.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <SDL.h>

// Custom includes
#include "common.h"

// C++ includes
#include <optional>
#include <vector>
#include <iostream>

class GameObject
{
public:
    GameObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    GameObject();

    ~GameObject();

    glm::vec2 get_position() const { return position; }
    glm::vec2 get_size() const { return size; }
    const std::string& get_texture_id() const { return texture_id; }

    std::optional<glm::vec3> get_colour() const { return colour; }
private:
    glm::vec2 position;
    glm::vec2 size;
    std::string texture_id;
    std::optional<glm::vec3> colour;
};

#endif
