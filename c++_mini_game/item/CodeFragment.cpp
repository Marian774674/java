#include "CodeFragment.h"

CodeFragment::CodeFragment(std::string keyword, FragmentCategory category,
                           Rarity rarity, std::string description)
    : Item(std::string("Frag_") + keyword, ItemType::CodeFragment, rarity, std::move(description))
    , m_keyword(std::move(keyword))
    , m_category(category)
{}

void CodeFragment::use() {
    // 碎片不能直接使用，需要在合成台中组合
}

FragmentCategory CodeFragment::categoryFromKeyword(const std::string& keyword) {
    // 关键字
    if (keyword == "for" || keyword == "while" || keyword == "if"
        || keyword == "else" || keyword == "switch" || keyword == "break"
        || keyword == "return" || keyword == "goto" || keyword == "case")
        return FragmentCategory::Keyword;
    // 类型
    if (keyword == "int" || keyword == "void" || keyword == "bool"
        || keyword == "const" || keyword == "char*" || keyword == "void*")
        return FragmentCategory::Type;
    // OOP
    if (keyword == "class" || keyword == "new" || keyword == "delete"
        || keyword == "this" || keyword == "virtual")
        return FragmentCategory::OOP;
    // 异常
    if (keyword == "try" || keyword == "catch" || keyword == "throw")
        return FragmentCategory::Exception;
    // 战斗动作
    if (keyword == "Attack" || keyword == "Defend" || keyword == "Heal"
        || keyword == "Counter")
        return FragmentCategory::Action;
    // 操作符
    return FragmentCategory::Operator;
}
