#include "Player.h"
#include "../game/EventBus.h"
#include "../item/CodeFragment.h"
#include "../item/Potion.h"
#include <algorithm>

Player::Player()
    : Entity(0, 0, '@', "程序员", START_HP, START_ATK, START_DEF)
    , m_energy(START_ENERGY)
    , m_maxEnergy(START_ENERGY)
    , m_xpToNext(XP_BASE)
{
    // 初始道具：一瓶回血药和一瓶能量饮料
    m_inventory.addItem(std::make_unique<Potion>(PotionType::HealthPotion));
    m_inventory.addItem(std::make_unique<Potion>(PotionType::EnergyDrink));

    // 初始碎片：几个基础关键字
    m_inventory.addItem(std::make_unique<CodeFragment>(
        "for", FragmentCategory::Keyword, Rarity::Common, "for 循环关键字"));
    m_inventory.addItem(std::make_unique<CodeFragment>(
        "if", FragmentCategory::Keyword, Rarity::Common, "if 条件关键字"));
    m_inventory.addItem(std::make_unique<CodeFragment>(
        "int", FragmentCategory::Type, Rarity::Common, "int 类型关键字"));
}

bool Player::move(int dx, int dy) {
    m_x += dx;
    m_y += dy;
    // 实际移动验证由 DungeonMap::isWalkable 处理
    return true;
}

void Player::spendEnergy(int amount) {
    if (m_godMode) return;  // 无敌模式：不消耗能量
    m_energy = std::max(0, m_energy - amount);
}

int Player::takeDamage(int rawDamage) {
    if (m_godMode) return 0;  // 无敌模式：免疫伤害
    // 使用含 Buff 和装备的有效防御
    int effectiveDef = getEffectiveDef();
    int actualDamage = std::max(1, rawDamage - effectiveDef);
    m_hp = std::max(0, m_hp - actualDamage);
    return actualDamage;
}

int Player::dealDamage() const {
    return getEffectiveAtk();
}

void Player::restoreEnergy(int amount) {
    m_energy = std::min(m_maxEnergy, m_energy + amount);
}

void Player::addXp(int amount) {
    m_xp += amount;
    while (checkLevelUp()) {
        // 升级持续触发，直到经验不足
    }
}

bool Player::checkLevelUp() {
    if (m_xp >= m_xpToNext) {
        m_xp -= m_xpToNext;
        ++m_level;
        m_xpToNext = static_cast<int>(m_xpToNext * 1.5);

        // 升级奖励
        m_maxHp += 20;
        m_hp = std::min(m_maxHp, m_hp + m_maxHp / 2);  // 回复 50% 生命
        m_atk += 5;
        m_def += 2;
        m_maxEnergy += 15;
        m_energy = std::min(m_maxEnergy, m_energy + m_maxEnergy / 2);  // 回复 50% 能量

        EventBus::instance().publish(GameEvent::LevelUp);
        return true;
    }
    return false;
}

void Player::applyBuff(int atkBonus, int defBonus, int turns) {
    m_tempAtkBonus = atkBonus;
    m_tempDefBonus = defBonus;
    m_buffTurnsRemaining = turns;
}

void Player::decrementBuffs() {
    if (m_buffTurnsRemaining > 0) {
        --m_buffTurnsRemaining;
        if (m_buffTurnsRemaining <= 0) {
            m_tempAtkBonus = 0;
            m_tempDefBonus = 0;
        }
    }
}

void Player::clearBuffs() {
    m_tempAtkBonus = 0;
    m_tempDefBonus = 0;
    m_buffTurnsRemaining = 0;
}

// 装备系统
bool Player::equip(std::unique_ptr<Equipment> eq) {
    if (!eq) return false;
    int slotIdx = static_cast<int>(eq->getSlot());

    // 如果已有同槽装备，卸下放回背包
    if (m_equipment[slotIdx]) {
        auto old = std::move(m_equipment[slotIdx]);
        m_inventory.addItem(std::move(old));
    }

    m_equipment[slotIdx] = std::move(eq);
    return true;
}

std::unique_ptr<Equipment> Player::unequip(EquipSlot slot) {
    int slotIdx = static_cast<int>(slot);
    if (!m_equipment[slotIdx]) return nullptr;
    return std::move(m_equipment[slotIdx]);
}

Equipment* Player::getEquipment(EquipSlot slot) const {
    return m_equipment[static_cast<int>(slot)].get();
}

int Player::getEquipmentAtkBonus() const {
    int total = 0;
    for (const auto& eq : m_equipment) {
        if (eq) total += eq->getAtkBonus();
    }
    return total;
}

int Player::getEquipmentDefBonus() const {
    int total = 0;
    for (const auto& eq : m_equipment) {
        if (eq) total += eq->getDefBonus();
    }
    return total;
}

int Player::getEquipmentHpBonus() const {
    int total = 0;
    for (const auto& eq : m_equipment) {
        if (eq) total += eq->getHpBonus();
    }
    return total;
}
