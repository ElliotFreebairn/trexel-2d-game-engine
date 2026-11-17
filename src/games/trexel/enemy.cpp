#include "enemy.h"
#include "game_object.h"

#include <cstdlib>
#include <chrono>
#include <numbers>

Enemy::Enemy(const GameObjectDef& def, glm::vec2 startingPosition)
    : GameObject(def, startingPosition)
{

}

void Enemy::update(float dt)
{
    static auto start = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

    std::cout << "Time elapsed: " << elapsed.count() << "\n";

    static glm::vec2 apply{};
    static bool first = false;

    int randomTime = rand() % 1500;
    if (elapsed.count() >= 1000 + randomTime || !first)
    {
        first = true;
        start = now;

        // randomly change direction - both x and y axis
        int randomNum = rand() % 100;

        int randomX = rand() % 100;
        int randomY = rand() % 100;

        if (randomNum < 50)
        {
            apply = {randomX, randomY};
        }
        else
        {
            apply = {-randomX, -randomY};
        }
    }

   transform.position += apply * dt;

   if (visual.sprite) visual.sprite->update(dt);
}

void Enemy::evCollided(CollisionAxis axis)
{

}
