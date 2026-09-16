#include "scene_object.h"

SceneObject::SceneObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour)
    : GameObject(position, size, texture_id, colour) {}

void SceneObject::update(float deltaTime)
{

}
