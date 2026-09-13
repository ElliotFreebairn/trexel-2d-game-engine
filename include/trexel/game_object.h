#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


// Library includes
#include <SDL_render.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <SDL.h>

// Custom includes
#include "common.h"
#include "IRenderable.h"
#include "sprite.h"

// C++ includes
#include <optional>
#include <vector>
#include <iostream>

class GameObject : public IRenderable
{
public:
    GameObject(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    GameObject();
    ~GameObject();

    /*
     * update will be called every frame
     */
    virtual void update(float dt) = 0;

    // getters
    glm::vec2 getPosition() const override { return position; }
    glm::vec2 getSize() const override { return size; }
    std::string getTextureID() const override;
    std::optional<Rect> getSourceRect() const override;
    std::optional<glm::vec3> getColour() const override { return colour; }

    // setters
    void setPosition(glm::vec2 position) { this->position = position; }

protected:
    std::optional<Sprite> sprite;

    glm::vec2 position;
    glm::vec2 size;
    std::string textureID;
    std::optional<glm::vec3> colour;

    void updateSprite(float dt) { if (sprite) sprite->update(dt); }
};

#endif
