#ifndef IRENDERABLE_H
#define IRENDERABLE_H

// Library includes
#include "glm/glm.hpp"

class IRenderable
{
    public:
        virtual glm::vec2 get_position() = 0;
        virtual glm::vec2 get_size() = 0;

        // temporary: should be texture
        virtual glm::vec3 get_colour() = 0;
};
#endif
