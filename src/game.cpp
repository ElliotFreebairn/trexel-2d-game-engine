#include "game.h"
#include "game_object.h"
#include <memory>

Game::Game()
{
    // Testing: Create squares
    glm::vec2 sq_pos_1(0.0f, 0.0f);
    glm::vec2 sq_size_1(100.0f, 100.0f);
    GameObject obj1 = {sq_pos_1, sq_size_1, "square", glm::vec3(300.0f, 255.0f, 0.0f)};


    glm::vec2 sq_pos_2(400.0f, 400.0f);
    glm::vec2 sq_size_2(50.0f, 50.0f);
    GameObject obj2 = {sq_pos_2, sq_size_2, "none"};

    game_objects.push_back(std::make_unique<GameObject>(obj1));
    game_objects.push_back(std::make_unique<GameObject>(obj2));
}

Game::~Game()
{

}
