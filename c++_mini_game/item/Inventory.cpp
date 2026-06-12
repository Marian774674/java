#include "Inventory.h"
#include <algorithm>
#include <set>

bool Inventory::isPermanentFragment(const std::string& keyword) {
    // "Attack" 是永久碎片，始终可用，不会被消耗
    // 这降低了所有包含 Attack 的配方的合成门槛
    static const std::set<std::string> permanent = {"Attack"};
    return permanent.count(keyword) > 0;
}

bool Inventory::addItem(std::unique_ptr<Item> item) {
    if (m_items.size() >= MAX_SLOTS) {
        return false;
    }
    m_items.push_back(std::move(item));
    return true;
}

std::unique_ptr<Item> Inventory::removeItem(int index) {
    if (index < 0 || static_cast<size_t>(index) >= m_items.size()) {
        return nullptr;
    }
    auto item = std::move(m_items[index]);
    m_items.erase(m_items.begin() + index);
    return item;
}

std::vector<CodeFragment*> Inventory::getFragments() const {
    std::vector<CodeFragment*> fragments;
    for (const auto& item : m_items) {
        if (item->getType() == ItemType::CodeFragment) {
            fragments.push_back(static_cast<CodeFragment*>(item.get()));
        }
    }
    return fragments;
}

bool Inventory::hasFragment(const std::string& keyword) const {
    if (isPermanentFragment(keyword)) return true;
    return countFragment(keyword) > 0;
}

int Inventory::countFragment(const std::string& keyword) const {
    int count = 0;
    for (const auto& item : m_items) {
        if (item->getType() == ItemType::CodeFragment) {
            auto* frag = static_cast<CodeFragment*>(item.get());
            if (frag->getKeyword() == keyword) {
                ++count;
            }
        }
    }
    return count;
}

int Inventory::removeFragments(const std::string& keyword, int count) {
    if (isPermanentFragment(keyword)) return count;  // 永久碎片不消耗，但返回"成功"
    int removed = 0;
    auto it = m_items.begin();
    while (it != m_items.end() && removed < count) {
        if ((*it)->getType() == ItemType::CodeFragment) {
            auto* frag = static_cast<CodeFragment*>(it->get());
            if (frag->getKeyword() == keyword) {
                it = m_items.erase(it);
                ++removed;
                continue;
            }
        }
        ++it;
    }
    return removed;
}
