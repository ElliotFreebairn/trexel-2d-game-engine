#include "scene_object.h"
#include "game_object.h"
#include "game_object_def.h"

SceneObject::SceneObject(const GameObjectDef& def, glm::vec2 position) : GameObject(def, position)
{
}

void SceneObject::update(const FrameContext& ctx)
{

}
