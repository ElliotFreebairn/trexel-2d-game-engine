#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


// Library includes
#include <glm/glm.hpp>

// Custom includes
#include "common.h"
#include "i_renderable.h"

class GameObject : IRenderable
{
public:
    GameObject(glm::vec2 position, glm::vec2 size, glm::vec3 colour);
    GameObject();

    ~GameObject();

    glm::vec2 get_position() override { return position; }
    glm::vec2 get_size() override { return size; }
    glm::vec3 get_colour() override { return colour; }
private:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 colour;
};

#endif
