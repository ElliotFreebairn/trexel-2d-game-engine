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

class IInspectable
{
public:
    virtual ~IInspectable() = default;

    virtual EntityId entityId() const = 0;
    virtual std::string typeName() const = 0;

    // Read: Describe every editable value
    virtual std::vector<Property> properties() const = 0;

    // Write: apply one edited value
    virtual void setProperty(std::string name, const PropertyValue& value)= 0;
};
