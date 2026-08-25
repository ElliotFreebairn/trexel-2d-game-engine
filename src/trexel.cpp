#include "trexel.h"
#include "IRenderable.h"
#include "player.h"
#include <vector>

void Trexel::onInit()
{

}

void Trexel::onUpdate(float deltaTime)
{

}

void Trexel::onShutdown()
{

}

std::vector<const IRenderable*> Trexel::getRenderables() const
{
    std::vector<const IRenderable*> renderables;

    if (player)
    {
        renderables.push_back(static_cast<IRenderable*>(player));
    }
    return renderables;
}

void Trexel::spawnPlayer(glm::vec2 pos)
{
    player = new Player(pos, {50.0f, 50.0f}, "grass_block");
}
