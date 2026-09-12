#include "game.h"
#include "IRenderable.h"
#include "game_object.h"
#include <memory>

std::vector<const IRenderable*> Game::getRenderables()
{
    std::vector<const IRenderable*> renderables;

    for(auto& obj : objects)
    {
        renderables.push_back(static_cast<const IRenderable*>(obj.get()));
    }
    return renderables;
}
