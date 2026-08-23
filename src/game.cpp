#include "game.h"
#include "game_object.h"
#include <memory>

Game::Game()
{
    // Testing: Create squares
    glm::vec2 sq_pos_1(0.0f, 0.0f);
    glm::vec2 sq_size_1(50.0f, 50.0f);
    GameObject obj1 = {sq_pos_1, sq_size_1, "grass_block", glm::vec3(300.0f, 255.0f, 0.0f)};


    glm::vec2 sq_pos_2(400.0f, 400.0f);
    glm::vec2 sq_size_2(50.0f, 50.0f);
    GameObject obj2 = {sq_pos_2, sq_size_2, "none", glm::vec3(255.0f, 0.0f, 0.0f)};

    game_objects.push_back(std::make_unique<GameObject>(obj1));
    game_objects.push_back(std::make_unique<GameObject>(obj2));
}

Game::~Game()
{

}

void Game::update()
{

}

void Game::spawn_block(glm::vec2 world_pos)
{
    glm::vec2 size{50.0f, 50.0f};
    GameObject obj = {world_pos, size, "grass_block"};
    game_objects.push_back(std::make_unique<GameObject>(obj));
}
