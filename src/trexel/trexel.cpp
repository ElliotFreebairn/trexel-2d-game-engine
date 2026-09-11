#include "trexel.h"
#include "IRenderable.h"
#include "game_actions.h"
#include "game_object.h"
#include "player.h"
#include <vector>

void Trexel::onInit()
{

}

void Trexel::onUpdate(float deltaTime)
{
    for (GameObject* obj : objects)
    {
        glm::vec2 pos = obj->getPosition();
        pos.x += 100 * deltaTime;
        obj->setPosition(pos);
    }
}

void Trexel::onShutdown()
{

}

std::vector<const IRenderable*> Trexel::getRenderables() const
{
    std::vector<const IRenderable*> renderables;

    for(auto obj : objects)
    {
        renderables.push_back(static_cast<IRenderable*>(obj));
    }
    return renderables;
}

void Trexel::handleAction(const GameAction& action)
{
    if (action.type == GameActionType::SpawnPlayer)
    {
        Player* player = new Player(action.position, {100.0f, 100.0f}, "grass_block");
        objects.push_back(static_cast<GameObject*>(player));
    }
}
