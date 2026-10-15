#pragma once

class SceneObject;

class IBehaviour
{
public:
    virtual ~IBehaviour() = default;
    virtual void update(SceneObject& owner, float dt) = 0;
};
