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

    for(auto obj : objects)
    {
        renderables.push_back(static_cast<IRenderable*>(obj));
    }
    return renderables;
}
