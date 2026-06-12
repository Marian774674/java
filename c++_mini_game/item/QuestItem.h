#pragma once
#include "Item.h"

// Quest items — NPC tokens dropped by specific enemies or from quests
class QuestItem : public Item {
public:
    QuestItem(std::string name, std::string desc, int questId)
        : Item(std::move(name), ItemType::QuestItem, Rarity::Uncommon, std::move(desc))
        , m_questId(questId)
    {}

    int getQuestId() const { return m_questId; }
    void use() override {}  // Turned in to NPCs

private:
    int m_questId;
};
