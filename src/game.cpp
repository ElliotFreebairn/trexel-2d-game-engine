#include "game.h"

Game::Game()
{
    // Testing: Create squares
    Position sq_pos_1 = {0, 0};
    Size sq_size_1 = {100, 100};

    Position sq_pos_2 = {400, 400};
    Size sq_size_2 = {50, 50};

    entities.push_back({sq_pos_1, sq_size_1});
    entities.push_back({sq_pos_2, sq_size_2});
}

Game::~Game()
{

}
