#pragma once
#include "Entity.h"
#include "../item/Inventory.h"
#include "../item/Equipment.h"
#include "../combat/SpellBook.h"
#include <memory>
#include <set>

// Player class - the programmer adventurer in the game
class Player : public Entity {
public:
    static constexpr int START_HP = 100;
    static constexpr int START_ATK = 15;
    static constexpr int START_DEF = 5;
    static constexpr int START_ENERGY = 100;
    static constexpr int XP_BASE = 200;

    Player();

    // 移动（返回是否成功移动）
    bool move(int dx, int dy);

    // 编译能量（施放咒语消耗）
    int getEnergy() const { return m_energy; }
    int getMaxEnergy() const { return m_maxEnergy; }
    void spendEnergy(int amount);
    void restoreEnergy(int amount);

    // 经验值
    int getXp() const { return m_xp; }
    int getXpToNext() const { return m_xpToNext; }
    void addXp(int amount);
    bool checkLevelUp();

    // 背包
    Inventory& getInventory() { return m_inventory; }
    const Inventory& getInventory() const { return m_inventory; }

    // 咒语书
    SpellBook* getSpellBook() { return &m_spellBook; }
    const SpellBook* getSpellBook() const { return &m_spellBook; }

    // 无敌模式（调试用）
    void setGodMode(bool on) { m_godMode = on; }
    bool isGodMode() const { return m_godMode; }

    // 任务追踪
    int getQuestsCompleted() const { return m_questsCompleted; }
    void addQuestCompleted() { ++m_questsCompleted; }
    int getGhostPages() const { return m_ghostPages; }
    void addGhostPage() { ++m_ghostPages; }
    int getFleeCount() const { return m_fleeCount; }
    void addFleeCount() { ++m_fleeCount; }
    int getSpellsCast() const { return m_spellsCast; }
    void addSpellCast() { ++m_spellsCast; }

    // 隐藏房间追踪
    int getHiddenRoomsFound() const { return m_hiddenRoomsFound; }
    void addHiddenRoomFound() { ++m_hiddenRoomsFound; }

    // 清空楼层追踪（完美清关结局）
    void markFloorCleared(int floor) { m_clearedFloors.insert(floor); }
    bool isFloorCleared(int floor) const { return m_clearedFloors.count(floor) > 0; }
    int getFloorsCleared() const { return static_cast<int>(m_clearedFloors.size()); }

    // 临时 Buff 系统
    void applyBuff(int atkBonus, int defBonus, int turns);
    void decrementBuffs();
    void clearBuffs();
    int getEffectiveAtk() const { return m_atk + m_tempAtkBonus + getEquipmentAtkBonus(); }
    int getEffectiveDef() const { return m_def + m_tempDefBonus + getEquipmentDefBonus(); }
    int getBuffTurnsRemaining() const { return m_buffTurnsRemaining; }

    // 装备系统
    bool equip(std::unique_ptr<Equipment> eq);
    std::unique_ptr<Equipment> unequip(EquipSlot slot);
    Equipment* getEquipment(EquipSlot slot) const;
    int getEquipmentAtkBonus() const;
    int getEquipmentDefBonus() const;
    int getEquipmentHpBonus() const;

    // 重写受击（无敌模式下免疫伤害）和攻击（含装备/Buff加成）
    int takeDamage(int rawDamage) override;
    int dealDamage() const override;

private:
    int m_energy;
    int m_maxEnergy;
    int m_xp = 0;
    int m_xpToNext;

    Inventory m_inventory;
    SpellBook m_spellBook;
    bool m_godMode = false;

    int m_questsCompleted = 0;   // Number of NPC quests completed
    int m_ghostPages = 0;        // Ghost diary pages collected
    int m_fleeCount = 0;         // Times fled from combat
    int m_spellsCast = 0;        // Times cast a spell
    int m_hiddenRoomsFound = 0;  // Hidden rooms discovered
    std::set<int> m_clearedFloors;  // 已清空所有怪物的楼层编号

    // 临时 Buff
    int m_tempAtkBonus = 0;
    int m_tempDefBonus = 0;
    int m_buffTurnsRemaining = 0;

    // 装备槽 [0]=Weapon, [1]=Armor, [2]=Accessory
    std::unique_ptr<Equipment> m_equipment[3];
};
