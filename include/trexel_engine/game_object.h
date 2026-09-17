#pragma once

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

    // IRenderable getters
    glm::vec2 getPosition() const override { return m_position; }
    glm::vec2 getSize() const override { return m_size; }
    std::string getTextureID() const override;
    std::optional<Rect> getSourceRect() const override;
    std::optional<glm::vec3> getColour() const override { return m_colour; }
    double getAngle() const override { return m_angle; }
    Flip getFlip() const override { return m_flip; }

    // GameObject getters
    bool isVisible() const { return m_isVisible; }
    bool isActive() const { return m_isActive; }

    // IRenderable setters
    void setPosition(glm::vec2 position) { m_position = position; }
    void setAngle(double angle) { m_angle = angle; }
    void setFlip(Flip flip) { m_flip = flip; }

    // GameObject setters
    void set_isVisible(bool visible) { m_isVisible = visible; }
    void setIsActive(bool active) { m_isActive = active; }

protected:
    std::optional<Sprite> m_sprite;
    bool m_isVisible = true;
    bool m_isActive = true;

    glm::vec2 m_position;
    glm::vec2 m_size;
    std::string m_textureId;
    std::optional<glm::vec3> m_colour;;
    double m_angle = 0.0;
    Flip m_flip = Flip::NONE;

    void updateSprite(float dt) { if (m_sprite) m_sprite->update(dt); }
};
