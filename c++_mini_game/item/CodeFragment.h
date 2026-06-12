#pragma once
#include "Item.h"
#include <string>

// 代码碎片类别
enum class FragmentCategory {
    Keyword,    // 关键字: for, while, if, else, switch, break, return, goto, case
    Type,       // 类型: int, void, bool, const, char*
    OOP,        // 面向对象: class, new, delete, this, virtual
    Exception,  // 异常: try, catch, throw
    Operator,   // 操作符: +, -, *, =, ==, !=, &&, ||, &
    Action      // 战斗动作: Attack, Defend, Heal, Counter
};

// 代码碎片——咒语合成的基础材料
class CodeFragment : public Item {
public:
    CodeFragment(std::string keyword, FragmentCategory category,
                 Rarity rarity, std::string description);

    std::string getKeyword() const { return m_keyword; }
    FragmentCategory getCategory() const { return m_category; }

    void use() override;  // 碎片本身不直接使用，在合成台中使用

    // 集中化：根据关键字字符串查询类别（避免各处重复 if-else 链）
    static FragmentCategory categoryFromKeyword(const std::string& keyword);

private:
    std::string m_keyword;      // 实际的代码关键字，如 "for"
    FragmentCategory m_category;
};
