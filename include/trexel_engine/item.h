#pragma once

#include <cstdint>
#include <string>
#include <utility>

using ItemId = std::uint32_t;

inline ItemId allocateItemId()
{
    static ItemId id = 1;
    return id++;
};

// Items should be a base class not a interface
class Item
{
public:
    Item(std::string name, std::string description, int quantity = 1) : m_id(allocateItemId()), m_name(name), m_quantity(quantity), m_description(description) {}

    virtual ~Item() = default;

    // getters
    ItemId id() const { return m_id; }
    std::string itemName() const { return m_name; }
    std::string description() const { return m_description; }
    int quantity() const { return m_quantity; }
    void setQuantity(int quantity) { m_quantity = quantity; }
private:
    ItemId m_id;
    std::string m_name;
    std::string m_description;
    int m_quantity;
};
