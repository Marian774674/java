#include "Item.h"

Item::Item(std::string name, ItemType type, Rarity rarity, std::string description)
    : m_name(std::move(name))
    , m_type(type)
    , m_rarity(rarity)
    , m_description(std::move(description))
{}
