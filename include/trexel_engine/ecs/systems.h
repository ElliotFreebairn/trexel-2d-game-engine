#pragma once

#include "../games/collision.h"

#include <vector>

class Registry;
class Scene;
struct InputState;

/*
 * All game behaviour lives here, as free functions over the registry
 * Scene::update() will call them in order
 *
 */

// brains: write Intent
void patrolAISystem(Registry& reg);
void playerInputSystem(Registry& reg, const InputState& input);

// bodies: read Intent
void locomotionSystem(Registry& reg);
void attackSystem(Scene& scene, float dt);

// the only place where positions are allowed to change
void physicsSystem(Registry& reg, float dt, std::vector<CollisionEvent>& collisions);

// reactions to the current frame
void patrolBounceSystem(Registry& reg, const std::vector<CollisionEvent>& collisions);
void projectileSystem(Registry& reg, float dt, const std::vector<CollisionEvent>& collisions);
void damageSystem(Registry& reg, const std::vector<CollisionEvent>& collisions);
void deathSystem(Registry& reg);

// visuals
void animationSystem(Registry& reg, float dt);
