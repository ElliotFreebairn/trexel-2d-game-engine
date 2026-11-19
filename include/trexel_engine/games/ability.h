#pragma once

#include "entity.h"

class Scene;

/*
 * Something an entity does when it attacks: fires an arrow, swings a sword etc...
 *
 * attackSystem decides WHEN (input, cooldown, animation frame), the ability decides WHAT
 */
class IAbility
{
public:
    virtual ~IAbility() = default;
    virtual void trigger(Scene& scene, EntityId user, float facing) = 0;
};
