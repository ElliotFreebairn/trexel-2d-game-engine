#ifndef PLAYER_H
#define PLAYER_H

// Custom includes
#include "game_object.h"
#include "sprite.h"

#include <optional>

// C++ includes
class Player : public GameObject
{
public:
    Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    Player();

    std::string getTextureID() const override { return "player_sheet"; }

    std::optional<SDL_Rect> getSourceRect() const override
    {
        return sprite.getSrcRect();
    }

    void move(glm::vec2 delta);

    virtual ~Player() = default;

    void update(float dt) override;

private:
    Sprite sprite {32, 32, 3, 1, 0.1};

};
#endif
