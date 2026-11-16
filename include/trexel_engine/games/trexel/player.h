#pragma once

// Custom includes
#include "ISprite.h"
#include "ability.h"
#include "bow_item.h"
#include "collision.h"
#include "game_object.h"
#include "game_object_def.h"

#include <optional>

class Scene;

// C++ include

/*
 * Any controllable character: walks, jumps, attacks with whatever ability
 */
class Player : public GameObject, public ISprite
{
public:
    Player(const GameObjectDef& def, glm::vec2 position);
    virtual ~Player() = default;

    void evCollided(CollisionAxis axis) override;

    // Custom player functions
    void move(glm::vec2 delta);
    void jump();
    void attack();

    // The ability fires on a specific animation frame, so the games asks when its ready
    bool wanstToTriggerAbility() const;
    void triggerAbility(Scene& scene);

    std::string typeName() const override { return "Player"; }
    glm::vec2 getFacing() const { return m_facing; }

    std::vector<Property> properties() const override;

    // Game object functions
    void update(float dt) override;
private:
    const CharacterStats& stats() const;

    std::unique_ptr<IAbility> m_ability; // null = wont attack

    bool m_isMoving = false;
    bool m_isJumping = false;
    bool m_isAttacking = false;
    bool m_hasTriggered = false;

    glm::vec2 m_facing = glm::vec2(1.0f, 0.0f);
    float m_fireCooldown = 0.0f;

    // Sprite functions
    void resolveSpriteState() override;
};
