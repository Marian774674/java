#include "Potion.h"

Potion::Potion(PotionType potionType)
    : Item(
        [potionType]() -> std::string {
            switch (potionType) {
                case PotionType::HealthPotion:      return "生命药水";
                case PotionType::EnergyDrink:       return "能量饮料";
                case PotionType::PowerPotion:       return "力量药剂";
                case PotionType::ShieldScroll:      return "护盾卷轴";
                case PotionType::ExperienceTome:    return "经验之书";
                case PotionType::TeleportScroll:    return "传送卷轴";
                case PotionType::PurificationPotion: return "净化药剂";
                default: return "未知药水";
            }
        }(),
        ItemType::Potion,
        [potionType]() -> Rarity {
            switch (potionType) {
                case PotionType::HealthPotion:
                case PotionType::EnergyDrink:
                    return Rarity::Common;
                case PotionType::PowerPotion:
                case PotionType::ShieldScroll:
                case PotionType::PurificationPotion:
                    return Rarity::Uncommon;
                case PotionType::ExperienceTome:
                case PotionType::TeleportScroll:
                    return Rarity::Rare;
                default: return Rarity::Common;
            }
        }(),
        [potionType]() -> std::string {
            switch (potionType) {
                case PotionType::HealthPotion:      return "恢复 30 点生命值";
                case PotionType::EnergyDrink:       return "恢复 40 点编译能量";
                case PotionType::PowerPotion:       return "临时 ATK+10，持续 3 回合";
                case PotionType::ShieldScroll:      return "临时 DEF+8，持续 3 回合";
                case PotionType::ExperienceTome:    return "获得 50~100 点经验值";
                case PotionType::TeleportScroll:    return "传送到当前层随机房间";
                case PotionType::PurificationPotion: return "清除所有临时效果";
                default: return "";
            }
        }()
    )
    , m_potionType(potionType)
{
    switch (potionType) {
        case PotionType::HealthPotion:
            m_healAmount = 30;
            break;
        case PotionType::EnergyDrink:
            m_energyAmount = 40;
            break;
        case PotionType::PowerPotion:
            m_atkBonus = 10;
            m_duration = 3;
            break;
        case PotionType::ShieldScroll:
            m_defBonus = 8;
            m_duration = 3;
            break;
        case PotionType::ExperienceTome:
            m_xpAmount = 50;  // 基础值，实际使用时会随机 50~100
            break;
        case PotionType::TeleportScroll:
        case PotionType::PurificationPotion:
            // 效果由调用方处理
            break;
    }
}

void Potion::use() {
    // 实际效果由调用方处理（需要访问 Player 对象）
}
