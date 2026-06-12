#pragma once
#include "Item.h"

// Potion types
enum class PotionType {
    HealthPotion,       // HP recovery
    EnergyDrink,        // Energy recovery
    PowerPotion,        // 临时 ATK+10, 3 回合
    ShieldScroll,       // 临时 DEF+8, 3 回合
    ExperienceTome,     // 获得经验值
    TeleportScroll,     // 传送到当前层随机房间
    PurificationPotion  // 清除负面/临时效果
};

// Potion - consumable item used in combat and exploration
class Potion : public Item {
public:
    Potion(PotionType potionType);

    void use() override;

    PotionType getPotionType() const { return m_potionType; }
    int getHealAmount() const { return m_healAmount; }
    int getEnergyAmount() const { return m_energyAmount; }
    int getAtkBonus() const { return m_atkBonus; }
    int getDefBonus() const { return m_defBonus; }
    int getDuration() const { return m_duration; }
    int getXpAmount() const { return m_xpAmount; }
    bool isTemporaryBuff() const {
        return m_potionType == PotionType::PowerPotion
            || m_potionType == PotionType::ShieldScroll;
    }

private:
    PotionType m_potionType;
    int m_healAmount = 0;
    int m_energyAmount = 0;
    int m_atkBonus = 0;
    int m_defBonus = 0;
    int m_duration = 0;
    int m_xpAmount = 0;
};
