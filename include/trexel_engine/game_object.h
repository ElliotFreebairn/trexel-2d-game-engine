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
    double getAngle() const override { return angle; }
    Flip getFlip() const override { return flip; }


    // setters
    void setPosition(glm::vec2 position) { this->position = position; }
    void setAngle(double angle) { this->angle = angle; }
    void setFlip(Flip flip) { this->flip = flip; }

protected:
    std::optional<Sprite> sprite;

    glm::vec2 position;
    glm::vec2 size;
    std::string textureID;
    std::optional<glm::vec3> colour;
    double angle = 0.0;
    Flip flip = Flip::NONE;

    void updateSprite(float dt) { if (sprite) sprite->update(dt); }
};

#endif
