#pragma once

// Custom includes
#include <glm/glm.hpp>
#include "common.h"
#include "collision.h"

// C++ includes
#include <string>
#include <optional>

class IRenderable
{
public:
    virtual ~IRenderable() = default;

    // Only expose what the renderer really needs
    virtual glm::vec2 getPosition() const = 0;
    virtual glm::vec2 getSize() const = 0;
    virtual std::string getTextureID() const = 0;
    virtual std::optional<glm::vec3> getColour() const = 0;

    // nullopt = use the whole texture
    virtual std::optional<Rect> getSourceRect() const { return std::nullopt; }

    virtual double getAngle() const = 0;
    virtual Flip getFlip() const = 0;

    // TESTING ONLY
    //virtual bool hasCollider() const { return false; }
    //virtual AAB getBounds() const { return { getPosition(), getPosition() + getSize() }; }};
};
