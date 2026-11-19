#pragma once

// Custom includes
#include "game_object.h"
#include "behaviour.h"
#include "game_object_def.h"

// C++ includes
#include <memory>
#include <vector>

class SceneObject : public GameObject
{
public:
    SceneObject(const GameObjectDef& def, glm::vec2 position);
    ~SceneObject() = default;

    void update(const FrameContext& ctx) override;

    void addBehaviour(std::unique_ptr<IBehaviour> b) { m_behaviours.push_back(std::move(b)); }

    std::string typeName() const override { return "SceneObject"; }
private:
    std::vector<std::unique_ptr<IBehaviour>> m_behaviours;
};
