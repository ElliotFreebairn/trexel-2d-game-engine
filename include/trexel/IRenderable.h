#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <glm/glm.hpp>
#include <string>
#include <optional>

/*
 * TO:DO - set to const, also question returning by value?
 */
class IRenderable
{
public:
    virtual ~IRenderable() = default;

    // Only expose what the renderer really needs
    virtual glm::vec2 getPosition() const = 0;
    virtual glm::vec2 getSize() const = 0;
    virtual std::string getTextureID() const = 0;
    virtual std::optional<glm::vec3> getColour() const = 0;
};

#endif
