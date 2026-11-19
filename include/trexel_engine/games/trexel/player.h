#pragma once

// Custom includes
#include "ability.h"
#include "bow_item.h"
#include "collision.h"
#include "game_object.h"
#include "game_object_def.h"
#include "sprite.h"

#include <optional>

class Scene;

// C++ include

/*
 * Any controllable character: walks, jumps, attacks with whatever ability
 */
class Player : public GameObject
{
public:
    Player(const GameObjectDef& def, glm::vec2 position);
    virtual ~Player() = default;

    std::string typeName() const override { return "Player"; }
    glm::vec2 getFacing() const { return m_facing; }

    std::vector<Property> properties() const override;

    // Game object functions
    void update(const FrameContext& ctx) override;
private:
    const CharacterStats& stats() const;

    std::unique_ptr<IAbility> m_ability; // null = wont attack

    bool m_isAttacking = false;
    bool m_hasTriggered = false;

    glm::vec2 m_facing = glm::vec2(1.0f, 0.0f);
    float m_cooldown = 0.0f;

    // Sprite functions
    SpriteMovementState animationState() override;
};
