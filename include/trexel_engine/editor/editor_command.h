#pragma once

#include "asset.h"
#include "entity.h"
#include "property.h"

#include <glm/glm.hpp>
#include <string>
#include <utility>
#include <vector>

// Things the editor UI can ask for
enum class EditorCommandType
{
    SetPlayMode,
    SetProperty,
    SpawnEntity,
    DestroyEntity,
    AddAsset
};

struct EditorCommand
{
    EditorCommandType type = EditorCommandType::SetPlayMode;

    bool playing = false; // SetPlayMode
    EntityId entity = INVALID_ENTITY; // SetProperty, DestoryEntity
    PropertyValue propertyValue; // SetProperty (name + new value)
    std::string propertyName;
    std::string prototype; // SpawnEntity
    glm::vec2 position = glm::vec2(0.0f); // SpawnEntity
    Asset asset;

    static EditorCommand setPlayMode(bool playing)
    {
        EditorCommand command;
        command.type = EditorCommandType::SetPlayMode;
        command.playing = playing;
        return command;
    }

    static EditorCommand setProperty(EntityId id, std::string name, PropertyValue value)
    {
        EditorCommand command;
        command.type = EditorCommandType::SetProperty;
        command.entity = id;
        command.propertyValue = std::move(value);
        command.propertyName = std::move(name);
        return command;
    }

    static EditorCommand spawnEntity(std::string definition, glm::vec2 position)
    {
        EditorCommand command;
        command.type = EditorCommandType::SpawnEntity;
        command.position = position;
        command.prototype = definition;
        return command;

    }

    static EditorCommand destroyEntity(EntityId id)
    {
        EditorCommand command;
        command.type = EditorCommandType::DestroyEntity;
        command.entity = id;
        return command;
    }

    static EditorCommand addAsset(Asset asset)
    {
        EditorCommand command;
        command.type = EditorCommandType::AddAsset;
        command.asset = std::move(asset);
        return command;
    }
    // Complete spawnEntity and destoryEntity
};

class CommandQueue
{
public:
    void push(EditorCommand command) { m_commands.push_back(std::move(command)); }

    std::vector<EditorCommand> flush()
    {
        std::vector<EditorCommand> out;
        out.swap(m_commands);
        return out;
    }

private:
    std::vector<EditorCommand> m_commands;
};
