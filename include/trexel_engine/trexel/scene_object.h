#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "../game_object.h"
#include "behaviour.h"
#include <memory>
#include <vector>

class SceneObject : public GameObject
{
public:
    SceneObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    SceneObject();
    ~SceneObject() = default;

    void update(float deltaTime) override;

    void addBehaviour(std::unique_ptr<IBehaviour> b) { mBehaviours.push_back(std::move(b)); }

private:
    std::vector<std::unique_ptr<IBehaviour>> mBehaviours;
};

#endif
