#pragma once
#include "Item.h"

// 装备槽位
enum class EquipSlot {
    Weapon,     // 武器
    Armor,      // 护甲
    Accessory   // 饰品
};

// 装备类 — 提供属性加成
class Equipment : public Item {
public:
    Equipment(std::string name, EquipSlot slot, Rarity rarity,
              int atkBonus, int defBonus, int hpBonus, std::string desc);

    EquipSlot getSlot() const { return m_slot; }
    int getAtkBonus() const { return m_atkBonus; }
    int getDefBonus() const { return m_defBonus; }
    int getHpBonus() const { return m_hpBonus; }

    void use() override {}  // 装备由背包交互菜单处理

private:
    EquipSlot m_slot;
    int m_atkBonus = 0;
    int m_defBonus = 0;
    int m_hpBonus = 0;
};
