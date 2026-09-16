#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "../game_object.h"

class SceneObject : public GameObject
{
public:
    SceneObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    SceneObject();

    ~SceneObject() = default;

    void update(float deltaTime) override;
private:

};

#endif
