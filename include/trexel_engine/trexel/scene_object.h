#pragma once

// Custom includes
#include "game_object.h"
#include "behaviour.h"

// C++ includes
#include <memory>
#include <vector>

class SceneObject : public GameObject
{
public:
    using GameObject::GameObject;

    ~SceneObject() = default;

    void update(float dt) override;

    void addBehaviour(std::unique_ptr<IBehaviour> b) { m_behaviours.push_back(std::move(b)); }
private:
    std::vector<std::unique_ptr<IBehaviour>> m_behaviours;
};
