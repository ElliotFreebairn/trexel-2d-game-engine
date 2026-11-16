#pragma once

#include <glm/glm.hpp>

class GameObject;
class Scene;

/*
 * Something the character does when it attacks: fires an arrow, swings a sword etc...
 *
 *  The character decides when (input, cooldown, animation frame)
 */
class IAbility
{
public:
    virtual ~IAbility() = default;

    virtual void trigger(GameObject& user, glm::vec2 facing, Scene& scene) = 0;
};
