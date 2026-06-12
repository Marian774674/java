#pragma once
#include "Item.h"

// Key item — drops from enemies, used to unlock stairs
class KeyItem : public Item {
public:
    explicit KeyItem(std::string keyName = "Rusty Key",
                     std::string desc = "A rusty key. Opens locked stairs.")
        : Item(std::move(keyName), ItemType::KeyItem, Rarity::Common, std::move(desc))
    {}

    void use() override {}  // Used via stairs interaction, not inventory
};
