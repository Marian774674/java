#include "SpellSystem.h"
#include "../entity/Player.h"
#include "../entity/Enemy.h"
#include "../item/Inventory.h"
#include "../item/CodeFragment.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <random>

SpellSystem::SpellSystem() {
    registerTemplates();
}

void SpellSystem::registerTemplates() {
    // ===== 1. for 循环连击 =====
    m_templates["for"] = {
        "for",
        "for(int i=0; i<N; i++) { Attack; }",
        "for 循环会将其循环体重复执行 N 次。"
        "在循环体中放入 \"Attack\" 指令会让编译器将其展开为 N 条连续的动作指令，"
        "在游戏中表现为连续攻击 N 次。",
        {"for", "int", "Attack"},
        [](int atk) { return atk * 3 / 4; },  // 每次攻击 75% 基础伤害
        []() { return 3; },                    // 默认连击 3 次
        25,                                    // 能量消耗
        "for 循环连击"
    };

    // ===== 2. if 条件防御反击 =====
    m_templates["if"] = {
        "if",
        "if(Enemy HP < 30%) { Counter; }",
        "if 是一种条件分支语句。"
        "系统首先评估条件 — 如果敌人生命值低于阈值，"
        "则触发反击；否则仅进行防御格挡。",
        {"if", "Counter"},
        [](int atk) { return atk * 2; },       // 反击伤害 2 倍
        []() { return 1; },                    // 单次
        20,
        "if 条件反击"
    };

    // ===== 3. const 常量暴击 =====
    m_templates["const"] = {
        "const",
        "const int Damage = 100; Attack * Damage;",
        "const 声明的变量在编译时确定且不可修改；"
        "编译器可以对其进行激进优化。在游戏中，这表现为一次暴击。",
        {"const", "int", "Attack", "*"},
        [](int atk) { return atk * 2; },       // 2倍暴击
        []() { return 1; },
        30,
        "const 常量暴击"
    };

    // ===== 4. class + new 构造函数召唤 =====
    m_templates["class"] = {
        "class",
        "class Ally { Ally() { Attack; } }; new Ally();",
        "class 定义了对象的蓝图，构造函数在对象创建时自动执行。"
        "new 分配内存并调用构造函数 — 召唤一个立即攻击的盟友。",
        {"class", "new", "Attack"},
        [](int atk) { return atk * 3 / 2; },   // 召唤伤害 1.5 倍
        []() { return 2; },                    // 召唤 + 本回合攻击 = 2击
        40,
        "class 构造函数召唤"
    };

    // ===== 5. try-catch 异常处理护盾 =====
    m_templates["try"] = {
        "try",
        "try { Defend; } catch(...) { Heal; }",
        "try 块尝试执行可能抛出异常的操作，"
        "catch 块捕获并处理异常。防御被视为可能失败的操作 → "
        "catch 将敌人的伤害转化为治疗。",
        {"try", "catch", "Defend", "Heal"},
        [](int atk) { return 0; },             // 攻击性=0，纯防御治疗
        []() { return 0; },
        35,
        "try-catch 护盾"
    };

    // ===== 6. delete 安全释放即死 =====
    m_templates["delete"] = {
        "delete",
        "if(Enemy.hp < 30%) { delete &Enemy; }",
        "delete 释放由 new 分配的内存。当敌人生命值低于 30% 时，"
        "delete 直接释放敌人对象 — 即死效果。"
        "& 取地址运算符确保我们操作的是敌人对象本身。",
        {"if", "delete", "&"},
        [](int) { return 9999; },              // 即死（如果HP<30%）
        []() { return 1; },
        50,
        "delete 即死"
    };

    // ===== 7. switch 分支语句轮盘 =====
    m_templates["switch"] = {
        "switch",
        "switch(Random) { case 1: Attack; break; case 2: Defend; break; default: Heal; }",
        "switch 根据随机值跳转到对应的 case；"
        "break 防止贯穿执行。每次使用随机施放一种技能效果。",
        {"switch", "break", "case", "Attack", "Defend", "Heal"},
        [](int atk) { return atk * 2; },       // 最大伤害
        []() { return 1; },
        20,
        "switch 分支轮盘"
    };

    // ===== 8. while 持续施压 =====
    m_templates["while"] = {
        "while",
        "while(Enemy Alive) { Attack; }",
        "while 循环会持续执行只要条件为真。"
        "它会不断攻击直到敌人倒下 — 持续施压造成额外伤害。",
        {"while", "Attack"},
        [](int atk) { return atk * 5 / 4; },   // 1.25倍 + 额外效果
        []() { return 1; },
        20,
        "while 持续施压"
    };

    // ===== 9. return 撤退反击 =====
    m_templates["return"] = {
        "return",
        "if(HP < 50%) { return Heal * 2; } return Attack;",
        "return 语句不仅结束函数，还可以携带返回值。"
        "在生命值低于 50% 时，return 返回 2 倍治疗；"
        "否则执行一次标准反击。战术性撤退法术。",
        {"return", "Heal", "Attack"},
        [](int atk) { return atk; },             // 基础 1x（HP<50%时恢复翻倍由castSpell处理）
        []() { return 1; },
        25,
        "return 撤退反击"
    };

    // ===== 10. throw 异常投掷 =====
    m_templates["throw"] = {
        "throw",
        "throw std::runtime_error(\"致命异常\");",
        "throw 语句抛出一个异常对象，"
        "将控制权转移到最近的 catch 处理程序。"
        "在战斗中表现为一次 2.5 倍的高伤害攻击——但每场战斗只能使用一次。",
        {"throw", "new", "try"},
        [](int atk) { return atk * 5 / 2; },     // 2.5x 高伤害
        []() { return 1; },
        45,
        "throw 异常投掷"
    };

    // ===== 11. virtual 虚函数重写 =====
    m_templates["virtual"] = {
        "virtual",
        "virtual void Attack() override { Counter; }",
        "virtual 函数允许子类重写（override）基类的行为。"
        "在战斗中，virtual 反射敌人上一回合造成的伤害，"
        "以彼之道还施彼身。",
        {"virtual", "class", "Attack"},
        [](int atk) { return atk * 3 / 2; },     // 1.5x 基础（反射伤害由castSpell处理）
        []() { return 1; },
        30,
        "virtual 虚函数重写"
    };

    // ===== 12. goto 跳转突袭 =====
    m_templates["goto"] = {
        "goto",
        "goto attack_label; attack_label: Attack * 3;",
        "goto 无条件跳转到指定标签。"
        "跳过了所有安全检查，直接执行 3 倍暴击——"
        "但本回合受到的伤害增加 50%。高风险高回报。",
        {"goto", "case", "Attack"},
        [](int atk) { return atk * 3; },          // 3x 暴击
        []() { return 1; },
        20,
        "goto 跳转突袭"
    };

    // ===== null pointer 空指针检查 =====
    // 此模板特殊处理，在 parse 中识别
}

SpellSystem::ParsedCode SpellSystem::parse(const std::string& input) const {
    ParsedCode result;

    // 简单的关键词提取
    // 检测主关键字
    std::vector<std::string> keywords = {
        "for", "while", "if", "switch", "try", "delete", "class", "const",
        "return", "throw", "virtual", "goto"
    };

    for (const auto& kw : keywords) {
        if (input.find(kw) != std::string::npos) {
            result.keyword = kw;
            break;
        }
    }

    // 检测碎片使用
    if (input.find("int") != std::string::npos) result.fragments.push_back("int");
    if (input.find("const") != std::string::npos) result.fragments.push_back("const");
    if (input.find("void") != std::string::npos) result.fragments.push_back("void");
    if (input.find("bool") != std::string::npos) result.fragments.push_back("bool");
    if (input.find("char") != std::string::npos) result.fragments.push_back("char*");
    if (input.find("class") != std::string::npos) result.fragments.push_back("class");
    if (input.find("new") != std::string::npos) result.fragments.push_back("new");
    if (input.find("delete") != std::string::npos) result.fragments.push_back("delete");
    if (input.find("this") != std::string::npos) result.fragments.push_back("this");
    if (input.find("virtual") != std::string::npos) result.fragments.push_back("virtual");
    if (input.find("try") != std::string::npos) result.fragments.push_back("try");
    if (input.find("catch") != std::string::npos) result.fragments.push_back("catch");
    if (input.find("throw") != std::string::npos) result.fragments.push_back("throw");
    if (input.find("switch") != std::string::npos) result.fragments.push_back("switch");
    if (input.find("break") != std::string::npos) result.fragments.push_back("break");
    if (input.find("case") != std::string::npos) result.fragments.push_back("case");
    if (input.find("return") != std::string::npos) result.fragments.push_back("return");
    if (input.find("goto") != std::string::npos) result.fragments.push_back("goto");
    if (input.find("nullptr") != std::string::npos) result.fragments.push_back("nullptr");
    if (input.find("else") != std::string::npos) result.fragments.push_back("else");
    if (input.find("while") != std::string::npos) result.fragments.push_back("while");
    if (input.find("for") != std::string::npos) result.fragments.push_back("for");
    if (input.find("if") != std::string::npos) result.fragments.push_back("if");

    // 操作符（只收集可掉落的碎片类型）
    if (input.find("==") != std::string::npos) result.fragments.push_back("==");
    else if (input.find("=") != std::string::npos) result.fragments.push_back("=");
    if (input.find("!=") != std::string::npos) result.fragments.push_back("!=");
    if (input.find("&&") != std::string::npos) result.fragments.push_back("&&");
    if (input.find("||") != std::string::npos) result.fragments.push_back("||");
    if (input.find("<") != std::string::npos) result.fragments.push_back("<");
    if (input.find(">") != std::string::npos) result.fragments.push_back(">");
    if (input.find("+") != std::string::npos) result.fragments.push_back("+");
    if (input.find("*") != std::string::npos) result.fragments.push_back("*");
    if (input.find("&") != std::string::npos) result.fragments.push_back("&");
    // 注意：; { } 是语法符号，不作为可收集碎片

    return result;
}

int SpellSystem::calculateEnergyCost(const ParsedCode& parsed) const {
    auto it = m_templates.find(parsed.keyword);
    if (it == m_templates.end()) return 30;  // 默认消耗

    int cost = it->second.baseEnergyCost;
    // 碎片越多，消耗略降（熟练度概念）
    cost = std::max(10, cost - static_cast<int>(parsed.fragments.size()));
    return cost;
}

int SpellSystem::calculateDamage(const ParsedCode& parsed, int baseAtk) const {
    auto it = m_templates.find(parsed.keyword);
    if (it == m_templates.end()) return baseAtk;

    return it->second.damageFunc(baseAtk);
}

int SpellSystem::calculateHits(const ParsedCode& parsed) const {
    auto it = m_templates.find(parsed.keyword);
    if (it == m_templates.end()) return 1;

    return it->second.hitsFunc();
}

std::string SpellSystem::explainEffect(const ParsedCode& parsed) const {
    auto it = m_templates.find(parsed.keyword);
    if (it == m_templates.end()) return "未知法术效果";

    return it->second.explanation;
}

SynthesisResult SpellSystem::synthesize(const std::string& codeInput,
                                         Player& player,
                                         Enemy* enemy) {
    SynthesisResult result;
    auto& inv = player.getInventory();

    // 1. 解析输入
    auto parsed = parse(codeInput);

    if (parsed.keyword.empty()) {
        result.success = false;
        result.message = "语法错误: 未识别到有效关键字。\n"
                        "请以以下关键字开头: for / if / while / switch / try / delete / class / const。";
        return result;
    }

    // 2. 检查模板是否存在
    auto it = m_templates.find(parsed.keyword);
    if (it == m_templates.end()) {
        result.success = false;
        result.message = "未知法术类型: " + parsed.keyword;
        return result;
    }

    // 3. 检查碎片是否足够
    std::vector<std::string> missingFrags;
    for (const auto& frag : parsed.fragments) {
        if (!inv.hasFragment(frag)) {
            missingFrags.push_back(frag);
        }
    }

    if (!missingFrags.empty()) {
        result.success = false;
        result.message = "碎片不足，缺少: ";
        for (size_t i = 0; i < missingFrags.size(); ++i) {
            result.message += "[" + missingFrags[i] + "]";
            if (i < missingFrags.size() - 1) result.message += ", ";
        }
        result.message += "\n请探索地牢收集更多代码碎片后再试。";
        return result;
    }

    // 4. 计算咒语属性
    int energyCost = calculateEnergyCost(parsed);
    int damage = calculateDamage(parsed, player.getAtk());
    int hits = calculateHits(parsed);
    std::string explanation = explainEffect(parsed);

    // 5. 消耗碎片
    for (const auto& frag : parsed.fragments) {
        inv.removeFragments(frag, 1);
    }

    // 6. 创建咒语
    LearnedSpell spell;
    spell.name = it->second.effectName;
    spell.codeTemplate = it->second.example;
    spell.energyCost = energyCost;
    spell.damage = damage;
    spell.effectDesc = "伤害: " + std::to_string(damage)
                     + " | 连击: " + std::to_string(hits)
                     + " | 能量: " + std::to_string(energyCost);

    result.success = true;
    result.spell = spell;
    result.message = "==== 编译成功! ====\n"
                    + spell.name + " 已添加到法术书!\n"
                    + spell.effectDesc + "\n\n"
                    + "--- 原理 ---\n"
                    + explanation + "\n\n"
                    + "--- 代码模板 ---\n"
                    + spell.codeTemplate;

    return result;
}

std::vector<std::string> SpellSystem::getTemplateKeys() const {
    // 返回按固定顺序排列的模板键（保证 UI 中配方序号一致）
    // 使用 static const 避免每次调用都分配新 vector
    static const std::vector<std::string> keys = {
        "for", "if", "const", "class", "try", "delete", "switch", "while",
        "return", "throw", "virtual", "goto"
    };
    return keys;  // 返回副本（调用方可能需要修改），但 string 数据共享
}

SynthesisResult SpellSystem::synthesize(int templateIndex, Player& player) {
    SynthesisResult result;
    auto& inv = player.getInventory();
    auto keys = getTemplateKeys();

    if (templateIndex < 0 || templateIndex >= static_cast<int>(keys.size())) {
        result.success = false;
        result.message = "无效的配方编号: " + std::to_string(templateIndex + 1);
        return result;
    }

    const auto& key = keys[templateIndex];
    auto it = m_templates.find(key);
    if (it == m_templates.end()) {
        result.success = false;
        result.message = "未找到模板: " + key;
        return result;
    }

    const auto& tmpl = it->second;

    // 检查所需碎片是否足够
    std::vector<std::string> missing;
    for (const auto& frag : tmpl.requiredFragments) {
        if (!inv.hasFragment(frag)) {
            missing.push_back(frag);
        }
    }

    if (!missing.empty()) {
        result.success = false;
        result.message = "缺少碎片: ";
        for (size_t i = 0; i < missing.size(); ++i) {
            result.message += "[" + missing[i] + "]";
            if (i < missing.size() - 1) result.message += ", ";
        }
        result.message += "\n请探索地牢打开宝箱来收集它们！";
        return result;
    }

    // 消耗碎片
    for (const auto& frag : tmpl.requiredFragments) {
        inv.removeFragments(frag, 1);
    }

    // 计算咒语属性
    int damage = tmpl.damageFunc(player.getAtk());
    int hits = tmpl.hitsFunc();
    int energyCost = tmpl.baseEnergyCost;

    LearnedSpell spell;
    spell.name = tmpl.effectName;
    spell.codeTemplate = tmpl.example;
    spell.energyCost = energyCost;
    spell.damage = damage;
    spell.effectDesc = "伤害: " + std::to_string(damage)
                     + " | 连击: " + std::to_string(hits)
                     + " | 能量: " + std::to_string(energyCost);

    result.success = true;
    result.spell = spell;
    result.message = "==== 编译成功! ====\n"
                    + spell.name + " 已添加到法术书!\n"
                    + spell.effectDesc + "\n\n"
                    + "--- 原理说明 ---\n"
                    + tmpl.explanation;

    return result;
}

std::string SpellSystem::castSpell(const LearnedSpell& spell,
                                    Player& player,
                                    Enemy& enemy) {
    // 检查能量
    if (player.getEnergy() < spell.energyCost) {
        return "编译能量不足！需要 " + std::to_string(spell.energyCost)
               + " 点，但当前只有 " + std::to_string(player.getEnergy()) + " 点。";
    }

    // 消耗能量
    player.spendEnergy(spell.energyCost);
    player.addSpellCast();

    std::string result = "施放法术 [" + spell.name + "]!\n";
    EnemyType eType = enemy.getEnemyType();
    int totalDamage = spell.damage;
    int finalDamage = 0;
    std::string counterMsg;

    // === 法术克制系统 ===
    if (spell.name == "for 循环连击") {
        if (eType == EnemyType::NullPointer) {
            // 无视闪避，必定命中
            enemy.setEvasionDisabled(true);
            counterMsg = "[克制] for循环展开覆盖所有空指针解引用路径，无视闪避！";
        } else if (eType == EnemyType::MemoryLeak) {
            // 连击撕裂，伤害+20%
            totalDamage = totalDamage * 120 / 100;
            counterMsg = "[克制] for循环反复释放内存，连击撕裂内存泄漏！伤害+20%";
        }
        // 多次攻击（连击3次）
        for (int hit = 0; hit < 3; ++hit) {
            if (!enemy.isAlive()) break;
            finalDamage += enemy.takeDamage(totalDamage);
        }
        // 重置闪避禁用标记
        enemy.setEvasionDisabled(false);
    }
    else if (spell.name == "const 常量暴击") {
        if (eType == EnemyType::Bug) {
            // 必定暴击 2x
            totalDamage = totalDamage * 2;
            counterMsg = "[克制] const编译期常量优化，对Bug必定暴击！";
        }
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "class 构造函数召唤") {
        // 通用效果：ATK+5 持续2回合
        player.applyBuff(5, 0, 2);
        counterMsg = "[效果] class构造召唤盟友，ATK+5 持续2回合！";
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "try-catch 护盾") {
        if (eType == EnemyType::Segfault) {
            // DEF+15 持续3回合
            player.applyBuff(0, 15, 3);
            counterMsg = "[克制] try-catch捕获Segfault信号，DEF+15 持续3回合！";
        } else {
            player.applyBuff(0, 8, 2);
            counterMsg = "[效果] try-catch护盾激活，DEF+8 持续2回合！";
        }
        // try-catch 为纯防御/治疗法术（0伤害）
        player.heal(20);
        result += "恢复了 20 点生命。";
    }
    else if (spell.name == "delete 即死") {
        if (enemy.getHp() < enemy.getMaxHp() * 3 / 10) {
            // HP<30% 真正即死
            totalDamage = 99999;
            counterMsg = "[即死] delete释放了敌人对象的内存！";
        } else {
            totalDamage = totalDamage / 4;  // HP>=30% 仅1/4伤害
            counterMsg = "[警告] 敌人HP>30%，delete仅造成少量伤害。";
        }
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "switch 分支轮盘") {
        // 随机效果
        std::mt19937 rng(std::random_device{}());
        int roll = std::uniform_int_distribution<int>(1, 4)(rng);
        switch (roll) {
            case 1: totalDamage = totalDamage * 150 / 100; counterMsg = "[分支1] 伤害+50%！"; break;
            case 2: player.heal(25); counterMsg = "[分支2] 恢复25点生命！"; break;
            case 3: player.restoreEnergy(20); counterMsg = "[分支3] 恢复20点能量！"; break;
            case 4: totalDamage = totalDamage * 3; counterMsg = "[分支4] 超暴击 3x！"; break;
        }
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "while 持续施压") {
        if (eType == EnemyType::StackOverflow) {
            totalDamage = totalDamage * 125 / 100;
            counterMsg = "[克制] while循环耗尽栈空间，对StackOverflow伤害+25%！";
        }
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "return 撤退反击") {
        if (player.getHp() < player.getMaxHp() / 2) {
            // HP<50% 恢复翻倍
            int healAmt = 30;
            player.heal(healAmt);
            counterMsg = "[效果] HP<50%，return返回治疗翻倍！恢复" + std::to_string(healAmt) + "点生命。";
        }
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "throw 异常投掷") {
        // 2.5x 高伤害（已在damageFunc中设置）
        counterMsg = "[效果] throw抛出致命异常，2.5倍伤害！";
        finalDamage = enemy.takeDamage(totalDamage);
    }
    else if (spell.name == "virtual 虚函数重写") {
        // 反射伤害：额外造成固定伤害
        int reflectDmg = player.getAtk();
        counterMsg = "[效果] virtual重写虚函数表，反射" + std::to_string(reflectDmg) + "点额外伤害！";
        finalDamage = enemy.takeDamage(totalDamage);
        if (enemy.isAlive()) {
            finalDamage += enemy.takeDamage(reflectDmg);
        }
    }
    else if (spell.name == "goto 跳转突袭") {
        // 3x 暴击，但本回合受伤+50%（由玩家takeDamage检查标记）
        totalDamage = totalDamage;  // damage已在damageFunc中设置为3x
        counterMsg = "[效果] goto跳过安全检查，3倍暴击！但本回合受伤+50%";
        finalDamage = enemy.takeDamage(totalDamage);
        // 标记本回合受伤+50%（通过临时降低DEF实现，简单处理）
        player.applyBuff(0, -player.getEffectiveDef() / 3, 1);
    }
    else {
        finalDamage = enemy.takeDamage(totalDamage);
    }

    if (finalDamage > 0) {
        result += "造成 " + std::to_string(finalDamage) + " 点伤害。";
    }
    if (!counterMsg.empty()) {
        result += "\n" + counterMsg;
    }
    result += " (能量 -" + std::to_string(spell.energyCost) + ")";

    return result;
}

std::vector<std::pair<std::string, std::string>> SpellSystem::getCodexEntries() const {
    std::vector<std::pair<std::string, std::string>> entries;

    for (const auto& [key, tmpl] : m_templates) {
        entries.push_back({tmpl.effectName, tmpl.explanation});
    }

    return entries;
}
