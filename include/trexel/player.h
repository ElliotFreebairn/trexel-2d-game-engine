#ifndef PLAYER_H
#define PLAYER_H

// Custom includes
#include "game_object.h"

// C++ includes
class Player : public GameObject
{
public:
    Player(glm::vec2 position, glm::vec2 size, std::string texture_id, std::optional<glm::vec3> colour = std::nullopt);
    Player();

    virtual ~Player() = default;

    void update() override;

private:

};
#endif
