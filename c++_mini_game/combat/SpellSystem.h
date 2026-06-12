#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "SpellBook.h"

class Player;
class Enemy;
class Inventory;

// 咒语合成结果
struct SynthesisResult {
    bool success = false;
    std::string message;
    LearnedSpell spell;
};

// 咒语系统——解析玩家输入的代码，合成咒语
class SpellSystem {
public:
    // 咒语模板数据结构（public，供 UI 使用）
    struct SpellTemplate {
        std::string keyword;              // 主关键字
        std::string example;              // 示例代码
        std::string explanation;          // 原理解释
        std::vector<std::string> requiredFragments;  // 所需碎片列表（可收集的）
        std::function<int(int baseAtk)> damageFunc;   // 伤害计算
        std::function<int()> hitsFunc;               // 连击数计算
        int baseEnergyCost;
        std::string effectName;
    };

    SpellSystem();

    // 尝试合成咒语（自由输入模式）
    SynthesisResult synthesize(const std::string& codeInput,
                               Player& player,
                               Enemy* enemy = nullptr);

    // 根据配方序号合成（配方选择模式）
    SynthesisResult synthesize(int templateIndex, Player& player);

    // 释放已学会的咒语
    std::string castSpell(const LearnedSpell& spell, Player& player, Enemy& enemy);

    // 获取合成教程（全部模板）
    std::vector<std::pair<std::string, std::string>> getCodexEntries() const;

    // 获取模板数据（供 UI 渲染配方列表）
    const std::map<std::string, SpellTemplate>& getTemplates() const { return m_templates; }
    std::vector<std::string> getTemplateKeys() const;

private:
    // 注册所有咒语模板
    void registerTemplates();

    // 解析玩家输入
    struct ParsedCode {
        std::string keyword;           // 主关键字
        std::vector<std::string> modifiers; // 修饰符
        std::vector<std::string> fragments;  // 用到的碎片
        std::string target;            // 目标
    };

    ParsedCode parse(const std::string& input) const;
    int calculateEnergyCost(const ParsedCode& parsed) const;
    std::string explainEffect(const ParsedCode& parsed) const;
    int calculateDamage(const ParsedCode& parsed, int baseAtk) const;
    int calculateHits(const ParsedCode& parsed) const;

    std::map<std::string, SpellTemplate> m_templates;
};
