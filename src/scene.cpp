#include "scene.h"
#include "collision.h"
#include "entity.h"
#include "game_object.h"
#include "game_object_def.h"
#include "object_defs.h"
#include "player.h"
#include "property.h"
#include "scene_object.h"
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Scene::update(float dt)
{
    for (auto& obj : m_objects)
    {
        if (obj->isActive())
        {
            obj->update(dt);
            obj->applyGravity(dt);
        }
    }

    pruneInactive();
}


/*
 * TEMP
 */
void Scene::saveScene(std::string mapPath)
{
    /*
     * JSON Format
     * {
     *    "objects": [
     *      {
     *          objectDef : "GrassBlock",
     *          colour : {
     *              ...
     *          },
     *          position : {
     *              ...
     *          },
     *          size : {
     *              ...
     *          }
     *      },
     *      {
     *          ...
     *      }
     *
     * }
     */

    json savedMap;
    for (const auto& objPtr : m_objects)
    {

        /*
         * create helper method glm::vecx to jsonobj, template that
         * also create a a struct that defines how store a json, making it easy to save and easier to read back
         */
        json className = {"class", objPtr->getClassName()};
        json objectDef = {"objectDef", objPtr->getObjectDefName()};
        json position = json::object({{"x", objPtr->getPosition().x}, {"y", objPtr->getPosition().y}});
        json size = json::object({{"width", objPtr->getSize().x}, {"height", objPtr->getSize().y}});
        json colour;

        // fix this
        if (objPtr->getColour().has_value())
        {
            colour = json::object({{{"r", objPtr->getColour().value().x}, {"g", objPtr->getColour().value().y}, {"b", objPtr->getColour().value().z}}});
        }

        json obj = json::object({className, objectDef, {"position", position}, {"size", size}, {"colour", colour}});
        savedMap["objects"] += obj;
    }

    // write to file
    std::fstream file;
    file.open(mapPath, std::ios::out);

    file << savedMap;
}

void Scene::loadScene(std::string mapPath)
{
    // load json from disk to json object
    std::fstream savedMapFile;
    savedMapFile.open(mapPath, std::ios::in);
    if (savedMapFile.fail()) return;

    json savedMap(json::parse(savedMapFile));
    if (savedMap["objects"].size() == 0) return;

    for (const auto& object : savedMap["objects"])
    {
        // we need to have a value of the type of game object, then potentially the game object def
        // redo how object def is stored in json, shoudn't be double idenetation
        std::string className = object["class"];
        std::string objectDefName = object["objectDef"];
        GameObjectDef objectDef = ObjectDefs::getGameObjectDef(objectDefName);
        glm::vec2 pos = {object["position"]["x"], object["position"]["y"]};
        glm::vec2 size = {object["size"]["width"], object["size"]["height"]};
        // parse colour

        // temp make templateable
        if (className == "Player")
        {
            // how do we attach the player
            // architectually this has to change?
            createObject<Player>(pos);
        }
        else if (className == "SceneObject")
        {
            createObject<SceneObject>(pos, objectDef);
        }

        std::cout << object["position"] << "\n";
    }
}
/*
 * TEMP
 */


void Scene::snapshotPositions()
{
    for (auto& objPtr : m_objects)
    {
        objPtr->setPrevPosition(objPtr->getPosition());
    }
}

void Scene::resolveCollisions(GameObject &mover)
{
    if (!mover.hasCollider()) return;

    for (auto& objPtr : m_objects)
    {
        GameObject* other = objPtr.get();
        if (other == &mover) continue;
        if (!other->hasCollider() || !other->getCollider()->isSolid) continue;

        AABB moverBounds = mover.getBounds();
        AABB otherBounds = other->getBounds();

        if (!aabbOverlap(moverBounds, otherBounds)) continue;

        glm::vec2 prevPos = mover.getPrevPosition();
        glm::vec2 currPos = mover.getPosition();

        // Isolate each axis: does moving *only* on that axis explain the overlap?
        bool xAlone = aabbOverlap(mover.getBoundsAt({currPos.x, prevPos.y}), otherBounds);
        bool yAlone = aabbOverlap(mover.getBoundsAt({prevPos.x, currPos.y}), otherBounds);

        float overlapX = std::min(moverBounds.max.x, otherBounds.max.x)
            - std::max(moverBounds.min.x, otherBounds.min.x);
        float overlapY = std::min(moverBounds.max.y, otherBounds.max.y)
            - std::max(moverBounds.min.y, otherBounds.min.y);

        bool resolveX;
        if (xAlone && !yAlone)      resolveX = true;   // clearly a side bump
        else if (yAlone && !xAlone) resolveX = false;  // clearly a floor/ceiling hit
        else                        resolveX = overlapX < overlapY; // ambiguous corner: fall back to old heuristic

        glm::vec2 pos = currPos;
        if (resolveX)
        {
            pos.x += (moverBounds.min.x < otherBounds.min.x) ? -overlapX : overlapX;
        }
        else
        {
            pos.y += (moverBounds.min.y < otherBounds.min.y) ? -overlapY : overlapY;
        }
        mover.setPosition(pos);
        mover.evCollided(resolveX ? CollisionAxis::X : CollisionAxis::Y);
    }
}

bool Scene::overlapsSolid(const GameObject &probe, EntityId ignore) const
{
    if (!probe.hasCollider()) return false;

    const AABB probeBounds = probe.getBounds();
    for (const auto& objPtr : m_objects)
    {
        const GameObject* other = objPtr.get();
        if (other == &probe) continue;
        if (ignore != INVALID_ENTITY && other->getEntityId() == ignore) continue;
        if (!other->hasCollider() || !other->getCollider()->isSolid) continue;

        if (aabbOverlap(probeBounds, other->getBounds())) return true;
    }
    return false;
}

bool Scene::isGroundedOn(const GameObject &probe, float skinWidth, EntityId ignore)
{
    if (!probe.hasCollider()) return false;

    AABB bounds = probe.getBounds();

    bounds.min.x += 2.0f;
    bounds.max.x -= 2.0f;

    bounds.min.y += skinWidth;
    bounds.max.y += skinWidth;

    for (const auto& objPtr : m_objects)
    {
        const GameObject* other = objPtr.get();
        if (other == &probe) continue;
        if (ignore != INVALID_ENTITY && other->entityId() == ignore) continue;
        if (!other->hasCollider() || !other->getCollider()->isSolid) continue;

        if (aabbOverlap(bounds, other->getBounds())) return true;
    }
    return false;
}

std::vector<const IRenderable*> Scene::getRenderables() const
{
    std::vector<const IRenderable*> out;
    out.reserve(m_objects.size());

    for (auto& obj : m_objects)
    {
        if (obj->isVisible())
        {
            out.push_back(obj.get());
        }
    }
    return out;
}

void Scene::pruneInactive()
{
    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(), [](const auto& o) { return !o->isActive(); }),
        m_objects.end());
}

std::vector<EntityInfo> Scene::getEntities() const
{
    std::vector<EntityInfo> out;
    out.reserve(m_objects.size());

    for (const auto& obj : m_objects)
    {
        if (!obj->isActive()) continue;

        out.push_back({obj->entityId(), obj->typeName() + " #" + std::to_string(obj->entityId())});
    }
    return out;
}

IInspectable* Scene::findInspectable(EntityId id) const
{
    if (id == INVALID_ENTITY) return nullptr;

    for (const auto& obj : m_objects)
    {
        if (obj->entityId() == id) return obj.get();
    }
    return nullptr;
}

EntityId Scene::pick(glm::vec2 worldPos) const
{
    // getRenderables() draws in list order, so the last object is on top.
    for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it)
    {
        GameObject* obj = it->get();
        if (!obj->isActive() || !obj->isVisible()) continue;

        // Return the closest object to the user mouse position
        const glm::vec2 pos  = obj->getPosition();
        const glm::vec2 size = obj->getSize();
        if (worldPos.x >= pos.x && worldPos.x < pos.x + size.x &&
            worldPos.y >= pos.y && worldPos.y < pos.y + size.y)
        {
            return obj->entityId();
        }
    }
    return INVALID_ENTITY;
}

void Scene::destroy(EntityId id)
{
    std::erase_if(m_objects, [id](const auto& obj) { return obj->entityId() == id; });
}
