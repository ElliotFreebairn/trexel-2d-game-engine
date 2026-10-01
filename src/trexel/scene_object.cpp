#include "scene_object.h"
#include "game_object.h"
#include "game_object_def.h"

SceneObject::SceneObject(glm::vec2 position, GameObjectDef objectDef) : GameObject(position, objectDef)
{
    m_className = __func__;
}

void SceneObject::update(float dt)
{

}
