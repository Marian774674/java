#pragma once
#include <vector>
#include <memory>
#include "Item.h"
#include "CodeFragment.h"

// 背包系统——存储玩家收集的物品和碎片
class Inventory {
public:
    static constexpr int MAX_SLOTS = 20;

    Inventory() = default;

    // 添加物品，成功返回 true，背包满返回 false
    bool addItem(std::unique_ptr<Item> item);

    // 移除物品（按索引）
    std::unique_ptr<Item> removeItem(int index);

    // 查询
    const std::vector<std::unique_ptr<Item>>& getItems() const { return m_items; }
    size_t getCount() const { return m_items.size(); }
    bool isFull() const { return m_items.size() >= MAX_SLOTS; }

    // 获取所有代码碎片
    std::vector<CodeFragment*> getFragments() const;

    // 检查是否拥有指定关键字的碎片
    bool hasFragment(const std::string& keyword) const;
    int countFragment(const std::string& keyword) const;

    // 移除指定关键字的碎片（返回移除数量）
    int removeFragments(const std::string& keyword, int count = 1);

    // 检查是否为永久碎片（不会被消耗）
    static bool isPermanentFragment(const std::string& keyword);

private:
    std::vector<std::unique_ptr<Item>> m_items;
};
