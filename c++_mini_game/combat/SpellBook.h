#pragma once
#include <string>
#include <vector>
#include <memory>

// 已学习的咒语条目
struct LearnedSpell {
    std::string name;           // 咒语名（自定义）
    std::string codeTemplate;   // 代码模板
    int energyCost;             // 编译能量消耗
    int damage;                 // 基础伤害
    std::string effectDesc;     // 效果描述
};

// 咒语书——存储已合成的咒语
class SpellBook {
public:
    static constexpr int MAX_SPELLS = 10;

    // 添加咒语
    bool addSpell(const LearnedSpell& spell);

    // 获取所有咒语
    const std::vector<LearnedSpell>& getSpells() const { return m_spells; }

    // 是否已满
    bool isFull() const { return m_spells.size() >= MAX_SPELLS; }

    // 是否有可用咒语
    bool hasSpells() const { return !m_spells.empty(); }

private:
    std::vector<LearnedSpell> m_spells;
};
