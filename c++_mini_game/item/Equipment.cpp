#include "Equipment.h"

Equipment::Equipment(std::string name, EquipSlot slot, Rarity rarity,
                     int atkBonus, int defBonus, int hpBonus, std::string desc)
    : Item(std::move(name), ItemType::Equipment, rarity, std::move(desc))
    , m_slot(slot)
    , m_atkBonus(atkBonus)
    , m_defBonus(defBonus)
    , m_hpBonus(hpBonus)
{}
