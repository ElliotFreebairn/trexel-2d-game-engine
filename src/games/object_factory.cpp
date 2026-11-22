#include "object_factory.h"
#include "ability_registry.h"
#include "collision.h"
#include "enum_strings.h"
#include "game_object_def.h"
#include "ecs/components.h"
#include "json_utility.h"
#include "sprite.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

/*
 * Object Definition JSON Example
 *
 * {
 *  "definitions":
 *      [
 *          {
 *              "name" : string, (example "archer")
 *              "textureId" : string, (example "archer_idle", if the animations are defined the texture is unused)
 *              "size": glm::vec2, (the size of the image)
 *              "layer": int, (draw order, higher is on top, default 0)
 *
 *              Every section below is optional, present = the entity gets that component
 *
 *              "body": { "gravity": bool }, (moves under physics)
 *              "movement": { "moveSpeed": float, "jumpForce": float },
 *              "attack": { "ability": string (registered name), "cooldown": float, "triggerFrame": int },
 *              "health": { "max": int },
 *              "projectile": { "lifetime": float },
 *              "damageOnHit": { "amount": int },
 *              "patrol": bool, (walks back and forth, turning at walls)
 *              "playerControlled": bool, (starts under player control)
 *              "collider" : (optional)
 *                  {
 *                      "size" : glm::vec2,  (default is the object size)
 *                      "offset": glm::vec2, (default {0, 0}),
 *                      "solid": bool (default true)
 *                  },
 *               "animations": (optional, states must match the SpriteMovementState enum), (frame size = size)
 *                {
 *                  "frameSpeed" : float, (default 0.2f)
 *                  "states" : [
 *                      {
 *                          "state" : "IDLE",
 *                          "textureId" : string,
 *                          "frames" : x,
 *                          "rows" : x, (default to 1),
 *                          "type" : "SINGULAR" etc.. (default is continous)
 *                      }
 *                  ]
 *                }
 *          }
 *      ]
 * }
 */

using json = nlohmann::json;

namespace
{
GameObjectDef parseDefinition(const json& defJson)
{
    GameObjectDef def;
    def.name = defJson.at("name").get<std::string>();
    def.visual.textureId = defJson.value("textureId", "");
    def.transform.size = vec2FromJson(defJson.at("size"));
    def.visual.layer = defJson.value("layer", 0);

    if (defJson.contains("collider"))
    {
        const json& cJson = defJson.at("collider");

        Collider collider;
        collider.size = cJson.contains("size") ? vec2FromJson(cJson.at("size")) : def.transform.size;
        collider.offset = cJson.contains("offset") ? vec2FromJson(cJson.at("offset")) : glm::vec2(0.0f);
        collider.isSolid = cJson.value("solid", true);

        def.collider = collider;
    }

    if (defJson.contains("animations"))
    {
        const json& aJson = defJson.at("animations");

        for (const auto& animation : aJson.at("states"))
        {
            SpriteAnimation spriteAnimation;
            spriteAnimation.textureID = animation.at("textureId").get<std::string>();
            spriteAnimation.columns = animation.at("frames").get<int>();
            spriteAnimation.rows = animation.value("rows", 1);

            auto type = fromString<AnimationType>(animation.at("type").get<std::string>());
            if (!type)
                throw std::runtime_error("Unkown animation type");
            spriteAnimation.animationType = *type;

            // each frame is the size of the object
            // ADD CHECK TO SEE IF FRAMEWIDTH AND/OR FRAMEHEIGHT ARE CUSTOMISED!
            spriteAnimation.frameWidth = static_cast<int>(def.transform.size.x);
            spriteAnimation.frameHeight = static_cast<int>(def.transform.size.y);

            // each frame can be defined by a generic frame speed or overriden in the state
            spriteAnimation.frameDuration = animation.value("frameSpeed", 0.2);

            auto state = fromString<SpriteMovementState>(animation.at("state").get<std::string>());
            if (!state)
            {
                // we should fail here
                throw std::runtime_error("Uknown animation state");
            }
            def.visual.animations[*state] = spriteAnimation;
        }
    }

    // components - defaults are defined in the structs
    if (defJson.contains("body"))
    {
        Body body;
        body.gravity = defJson.at("body").value("gravity", body.gravity);
        def.body = body;
    }

    if (defJson.contains("movement"))
    {
        const json& j = defJson.at("movement");
        Movement movement;
        movement.moveSpeed = j.value("moveSpeed", movement.moveSpeed);
        movement.jumpForce = j.value("jumpForce", movement.jumpForce);
        def.movement = movement;
    }

    if (defJson.contains("attack"))
    {
        const json& j = defJson.at("attack");
        AbilityDef attack;
        attack.name = j.at("ability").get<std::string>();
        attack.cooldown = j.value("cooldown", attack.cooldown);
        attack.triggerFrame = j.value("triggerFrame", attack.triggerFrame);
        def.attack = attack;
    }

    if (defJson.contains("health"))
    {
        const int max = defJson.at("health").value("max", Health{}.max);
        def.health = Health{max, max};
    }

    if (defJson.contains("projectile"))
    {
        Projectile projectile;
        projectile.lifetime = defJson.at("projectile").value("lifetime", projectile.lifetime);
        def.projectile = projectile;
    }

    if (defJson.contains("damageOnHit"))
    {
        def.damageOnHit = DamageOnHit{defJson.at("damageOnHit").value("amount", DamageOnHit{}.amount)};
    }

    if (def.attack)
    {
        auto it = def.visual.animations.find(SpriteMovementState::ATTACKING);
        if (it != def.visual.animations.end() && def.attack->triggerFrame >= it->second.columns)
        {
            std::cerr << "ObjectFactory: \"" << def.name << "\" attack triggerFrame " << def.attack->triggerFrame
                      << " is never reached, ATTACKING has frames 0-" << it->second.columns - 1 << "\n";
        }
    }

    // Tags
    def.patrol = defJson.value("patrol", false);
    def.playerControlled = defJson.value("playerControlled", false);

    return def;
}

json parseDefinition(const GameObjectDef& def)
{
    json saved;
    saved["name"] = def.name;
    saved["textureId"] = def.visual.textureId.empty() ? "" : def.visual.textureId;
    saved["size"] = vec2ToJson(def.transform.size);
    if (def.visual.layer != 0) saved["layer"] = def.visual.layer;

    // components - optional definitions
    if (def.collider.has_value())
    {
        json collider;
        collider["size"] = vec2ToJson(def.collider->size);
        collider["offset"] = vec2ToJson(def.collider->offset);
        collider["solid"] = def.collider->isSolid;

        saved["collider"] = collider;
    }

    if (def.visual.animations.size() > 0)
    {
        json animations;
        for (const auto&[state, animation] : def.visual.animations)
        {
            json st;
            st["textureId"] = animation.textureID;
            st["frames"] = animation.columns;
            st["rows"] = animation.rows;
            st["type"] = toString(animation.animationType);
            st["frameWidth"] = animation.frameWidth != - 1 ? animation.frameWidth : def.transform.size.x; // frameWidth is custom
            st["frameHeight"] = animation.frameHeight != - 1 ? animation.frameHeight : def.transform.size.y; // frameHieght is custom
            st["frameDuration"] = animation.frameDuration;
            st["state"] = toString(state);
            animations["states"] += st;
        }
        saved["animations"] = animations;
    }

    // components are only written if they are present
    if (def.body) {
        saved["body"] = {{"gravity", def.body->gravity}};
    }
    if (def.movement)
    {
        saved["movement"] = {{"moveSpeed", def.movement->moveSpeed}, {"jumpForce", def.movement->jumpForce}};
    }
    if (def.attack)
    {
        saved["attack"] = {{"ability", def.attack->name},
            {"cooldown", def.attack->cooldown},
            {"triggerFrame", def.attack->triggerFrame}};
    }
    if (def.health) {
        saved["health"] = {{"max", def.health->max}};
    }
    if (def.projectile) {
        saved["projectile"] = {{"lifetime", def.projectile->lifetime}};
    }
    if (def.damageOnHit) {
        saved["damageOnHit"] = {{"amount", def.damageOnHit->amount}};
    }
    if (def.patrol) {
        saved["patrol"] = true;
    }
    if (def.playerControlled) {
        saved["playerControlled"] = true;
    }
    return saved;
}
}

bool ObjectFactory::loadDefinitions(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ObjectFactory::loadDefinitions: cannot open " << path << "\n";
        return false;
    }

    json loaded = json::parse(file, nullptr, /*allow exceptions*/ false);
    if (loaded.is_discarded() || !loaded.contains("definitions"))
    {
        std::cerr << "ObjectFactory::loadDefinitions: " << path << " isn't valid or has no \"definitions\"\n";
        return false;
    }

    for (const auto& defJson : loaded["definitions"])
    {
        try
        {
            GameObjectDef def = parseDefinition(defJson);
            resolveAbility(def);
            m_definitions[def.name] = def;
        }
        catch (const json::exception& e)
        {
            std::cerr << "ObjectFactory::loadDefinitions: skipping a definition, " << e.what() << "\n";
        }
    }
    return true;
}

// potentially timestamp and only save definitions has been modified
bool ObjectFactory::saveDefinitions(const std::string &path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ObjectFactory::saveDefinitions: cannot open " << path << "\n";
        return false;
    }

    if (m_definitions.empty())
    {
        std::cerr << "ObjectFactory::saveDefinitions: not definitions to save" << "\n";
        return false;
    }

    constexpr const char* JSON_HEADER = "definitions";
    json saved;

    // go through each entry in the definitions map
    for (const auto&[id, def] : m_definitions)
    {
        try
        {
            json definition = parseDefinition(def);
            saved[JSON_HEADER] += definition;
        }
        catch (const std::exception)
        {
            std::cerr << "ObjectFactory::saveDefinitions: " << "\n";
            return false;
        }
    }

    file << saved.dump(4);
    return true;
}

const GameObjectDef* ObjectFactory::getDefinition(const std::string& name) const
{
    auto it = m_definitions.find(name);
    return it != m_definitions.end() ? &it->second : nullptr;
}

EntityId ObjectFactory::spawn(Registry& reg, const std::string& defName, glm::vec2 position) const
{
    const GameObjectDef* def = getDefinition(defName);
    if (!def)
    {
        std::cerr << "ObjectFactory::spawn: no definition called \"" << defName << "\"\n";
        return INVALID_ENTITY;
    }

    EntityId e = reg.create();

    // every entity has a prefab, transform and visual component
    reg.add<Prefab>(e, {def->name});
    reg.add<Transform>(e, {.position = position, .size = def->transform.size});

    Visual visual;
    visual.textureId = def->visual.textureId;
    visual.colour = def->visual.colour;
    visual.layer = def->visual.layer;
    if (!def->visual.animations.empty())
    {
        visual.sprite = Sprite();
        for (const auto& [state, animation] : def->visual.animations)
        {
            visual.sprite->addAnimation(state, animation);
        }
    }
    reg.add<Visual>(e, std::move(visual));

    // optional data components
    if (def->collider)
    {
        reg.add<Collider>(e, *def->collider);
    }
    if (def->body)
    {
        reg.add<Body>(e, *def->body);
    }
    if (def->movement)
    {
        reg.add<Movement>(e, *def->movement);
    }
    if (def->health)
    {
        reg.add<Health>(e, *def->health);
    }
    if (def->projectile)
    {
        reg.add<Projectile>(e, *def->projectile);
    }
    if (def->damageOnHit)
    {
        reg.add<DamageOnHit>(e, *def->damageOnHit);
    }

    // tags
    if (def->patrol)
    {
        reg.add<PatrolAI>(e);
    }
    if (def->playerControlled)
    {
        reg.add<PlayerControlled>(e);
    }

    // attacks need a fresh ability per entity
    if (def->attack)
    {
        Attack attack;
        attack.ability = def->attack->make();
        attack.cooldown = def->attack->cooldown;
        attack.triggerFrame = def->attack->triggerFrame;
        reg.add<Attack>(e, std::move(attack));
    }

    // anything that can act needs an intent for us to write to. Not to be saved in JSON!!!
    if (def->movement || def->attack || def->patrol || def->playerControlled)
    {
        reg.add<Intent>(e);
    }

    return e;
}

std::vector<std::string> ObjectFactory::getDefinitionsKeys() const
{
    std::vector<std::string> definitions;

    std::for_each(m_definitions.begin(), m_definitions.end(), [&](std::pair<std::string, GameObjectDef> p) {
       definitions.push_back(p.first);
    });

    return definitions;
}

std::vector<GameObjectDef> ObjectFactory::getDefinitionsValues() const
{
    std::vector<GameObjectDef> definitions;

    std::for_each(m_definitions.begin(), m_definitions.end(), [&](std::pair<std::string, GameObjectDef> p) {
        definitions.push_back(p.second);
    });

    return definitions;
}

bool ObjectFactory::addDefinition(const GameObjectDef &def)
{
    GameObjectDef resolved = def;
    try
    {
        resolveAbility(resolved);
    }
    catch (const std::exception& e)
    {
        std::cerr << "ObjectFactory::addDefinition: " << e.what() << "\n";
        return false;
    }

    m_definitions[resolved.name] = std::move(resolved);
    return true;
}

void ObjectFactory::resolveAbility(GameObjectDef &def) const
{
    if (!def.attack) return;

    AbilityDef& ability = *def.attack;
    const AbilityRegistry::Maker* maker = m_abilities.find(ability.name);

    if (!maker)
    {
        throw std::runtime_error("unkown ability \"" + ability.name + "\" in definition \"" + def.name + "\"");
    }
    ability.make = *maker;
}
