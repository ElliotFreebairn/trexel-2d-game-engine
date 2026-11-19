#pragma once

#include "entity.h"

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <vector>

// One named, editable value
using PropertyValue = std::variant<bool, int, float, glm::vec2, std::string>;

struct Property
{
    std::string name;
    PropertyValue value;
    bool readOnly = false;
};

// A snapshot of one entity for the inspector. Edits go back through EditorCommand::SetProperty
struct InspectorData
{
    EntityId id = INVALID_ENTITY;
    std::string name;
    std::vector<Property> properties;
};
