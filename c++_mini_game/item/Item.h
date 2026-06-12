#pragma once
#include <string>

// 物品稀有度
enum class Rarity {
    Common,     // 普通
    Uncommon,   // 稀有
    Rare,       // 极稀有
    Legendary   // 传说
};

// 物品类型
enum class ItemType {
    CodeFragment,  // 代码碎片
    Potion,        // 药水
    KeyItem,       // 钥匙
    QuestItem,     // 任务信物
    Equipment      // 装备（预留）
};

// 物品基类
class Item {
public:
    Item(std::string name, ItemType type, Rarity rarity, std::string description);
    virtual ~Item() = default;

    std::string getName() const { return m_name; }
    ItemType getType() const { return m_type; }
    Rarity getRarity() const { return m_rarity; }
    std::string getDescription() const { return m_description; }

    virtual void use() = 0;

protected:
    std::string m_name;
    ItemType m_type;
    Rarity m_rarity;
    std::string m_description;
};
