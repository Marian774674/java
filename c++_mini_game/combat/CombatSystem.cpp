#include "CombatSystem.h"
#include "Command.h"
#include "SpellSystem.h"
#include "SpellBook.h"
#include "../entity/Player.h"
#include "../entity/Enemy.h"
#include "../game/GameEngine.h"
#include "../game/Renderer.h"
#include "../game/EventBus.h"
#include "../item/Potion.h"
#include "../item/Equipment.h"
#include "../item/KeyItem.h"
#include "../item/QuestItem.h"
#include "../item/CodeFragment.h"
#include <algorithm>
#include <random>

CombatSystem::CombatSystem() = default;
CombatSystem::~CombatSystem() = default;  // must be here where CombatCommand is complete

void CombatSystem::startCombat(Player& player, Enemy& enemy) {
    m_player = &player;
    m_enemy = &enemy;
    m_result = CombatResult::Ongoing;
    m_turnCount = 0;
    m_playerDefending = false;
    m_battleLog.clear();

    // 清空历史（新战斗）
    while (!m_commandHistory.empty()) m_commandHistory.pop();

    m_battleLog.push_back("⚔ 遭遇 " + enemy.getName() + "！进入战斗！");
}

void CombatSystem::render(GameEngine& engine) {
    if (!m_player || !m_enemy) return;
    if (m_result != CombatResult::Ongoing) return;

    auto& renderer = engine.getRenderer();
    const int L = 1, R = 79, T = 1, B = 29;  // Full screen
    const int INNER_L = L + 1, INNER_R = R - 1;  // 2..78
    const int BOUND_R = R - 2;  // 77 safe bound

    renderer.drawBox(L, T, R, B, Color::Red);

    // === Enemy info ===
    renderer.drawStringBounded(4, 3, BOUND_R,
        m_enemy->getName() + "  Lv." + std::to_string(m_enemy->getLevel()),
        Color::Red);
    renderer.drawString(4, 4, "HP:", Color::White);
    renderer.drawBar(7, 4, 22, m_enemy->getHp(), m_enemy->getMaxHp(),
                     Color::Red, Color::DarkGray);
    renderer.drawStringBounded(30, 4, BOUND_R,
        std::to_string(m_enemy->getHp()) + "/" + std::to_string(m_enemy->getMaxHp()),
        Color::White);

    std::string specialDesc = m_enemy->getSpecialDescription();
    if (!specialDesc.empty()) {
        renderer.drawStringBounded(4, 5, BOUND_R, "[特性] " + specialDesc, Color::Magenta);
    }

    // === Player info ===
    renderer.drawString(4, 7, m_player->getName(), Color::Green);
    renderer.drawStringBounded(20, 7, BOUND_R,
        "Lv." + std::to_string(m_player->getLevel()), Color::Yellow);
    renderer.drawString(4, 8, "HP:", Color::White);
    renderer.drawBar(7, 8, 22, m_player->getHp(), m_player->getMaxHp(),
                     Color::Green, Color::DarkGray);
    renderer.drawStringBounded(30, 8, 50,
        std::to_string(m_player->getHp()) + "/" + std::to_string(m_player->getMaxHp()),
        Color::White);
    renderer.drawString(45, 8, "能量:", Color::White);
    renderer.drawBar(52, 8, 15, m_player->getEnergy(), m_player->getMaxEnergy(),
                     Color::Blue, Color::DarkGray);
    renderer.drawStringBounded(68, 8, BOUND_R,
        std::to_string(m_player->getEnergy()) + "/" + std::to_string(m_player->getMaxEnergy()),
        Color::White);

    // Buff 状态
    if (m_player->getBuffTurnsRemaining() > 0) {
        std::string buffStr = "[Buff] ATK:" + std::to_string(m_player->getEffectiveAtk())
            + " DEF:" + std::to_string(m_player->getEffectiveDef())
            + " (剩余 " + std::to_string(m_player->getBuffTurnsRemaining()) + " 回合)";
        renderer.drawStringBounded(4, 9, BOUND_R, buffStr, Color::Cyan);
    }

    // === Divider ===
    renderer.drawString(INNER_L, 10, std::string(76, '='), Color::DarkGray);

    // === Battle Log (12 lines: y=11..22) ===
    int logStart = std::max(0, static_cast<int>(m_battleLog.size()) - 12);
    int logY = 11;
    for (size_t i = logStart; i < m_battleLog.size() && logY < 23; ++i) {
        renderer.drawStringBounded(3, logY++, BOUND_R, " " + m_battleLog[i], Color::Yellow);
    }

    // === Divider ===
    renderer.drawString(INNER_L, 23, std::string(76, '-'), Color::DarkGray);

    // === Actions (2 columns) ===
    renderer.drawString(6, 24, "[1] 攻击         [2] 防御", Color::White);
    renderer.drawString(6, 25, "[3] 施法         [S] 法术选择", Color::White);
    renderer.drawString(6, 26, "[4] 使用物品     [I] 打开背包", Color::White);
    renderer.drawString(6, 27, "[5] 逃跑         [F] 逃跑", Color::White);
    renderer.drawStringBounded(6, 28, 40, "[C] 合成台  [U] 撤销", Color::White);
    renderer.drawStringBounded(50, 28, BOUND_R, "[Esc] 放弃", Color::DarkGray);

    renderer.drawString(3, Renderer::CONSOLE_HEIGHT - 1, "▶ 选择行动:", Color::Cyan);
}

void CombatSystem::handleInput(GameEngine& engine, int key) {
    if (!m_player || !m_enemy) return;
    if (m_result != CombatResult::Ongoing) return;

    auto& renderer = engine.getRenderer();

    if (key == '1') {
        auto cmd = std::make_unique<AttackCommand>();
        std::string msg = cmd->execute(*m_player, *m_enemy);
        m_battleLog.push_back(msg);
        pushCommand(std::move(cmd));
        processEnemyTurn();
        checkCombatEnd();
    }
    else if (key == '2') {
        auto cmd = std::make_unique<DefendCommand>();
        std::string msg = cmd->execute(*m_player, *m_enemy);
        m_battleLog.push_back(msg);
        pushCommand(std::move(cmd));
        processEnemyTurn();
        checkCombatEnd();
    }
    else if (key == '3' || key == 's' || key == 'S') {
        auto& spellSystem = engine.getSpellSystem();
        auto& spellBook = *m_player->getSpellBook();
        const auto& spells = spellBook.getSpells();

        if (!spellBook.hasSpells()) {
            m_battleLog.push_back("尚未学习法术！打开背包按 C 进入合成台。");
        } else {
            // Spell selection sub-screen
            renderer.clear();
            renderer.drawBox(3, 3, 77, 27, Color::Magenta);
            renderer.drawCentered(3, "─── 选择法术 ───", Color::Yellow);
            for (size_t i = 0; i < spells.size() && i < 10; ++i) {
                std::string entry = "[" + std::to_string(i + 1) + "] "
                    + spells[i].name + "  |  " + spells[i].effectDesc;
                renderer.drawStringBounded(6, 6 + static_cast<int>(i), 75,
                    entry, (i % 2 == 0) ? Color::White : Color::Cyan);
            }
            renderer.drawStringBounded(6, 6 + static_cast<int>(spells.size()) + 2, 75,
                "按数字键选择，[Esc] 返回", Color::DarkGray);
            renderer.present();

            int spellKey = renderer.waitForKey();
            int spellIdx = spellKey - '1';
            if (spellIdx >= 0 && static_cast<size_t>(spellIdx) < spells.size()) {
                auto& spell = spells[spellIdx];
                std::string msg = spellSystem.castSpell(spell, *m_player, *m_enemy);
                m_battleLog.push_back(msg);
                auto cmd = std::make_unique<AttackCommand>();
                pushCommand(std::move(cmd));
                processEnemyTurn();
                checkCombatEnd();
            }
        }
    }
    else if (key == '4' || key == 'i' || key == 'I') {
        auto& inv = m_player->getInventory();
        auto& items = inv.getItems();
        std::vector<int> potionIdx;
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->getType() == ItemType::Potion)
                potionIdx.push_back(static_cast<int>(i));
        }
        if (potionIdx.empty()) {
            m_battleLog.push_back("背包中没有可用物品。");
        } else {
            // 显示药水选择菜单
            renderer.clear();
            renderer.drawBox(5, 5, 75, 25, Color::Cyan);
            renderer.drawCentered(5, "─── 选择物品 ───", Color::Yellow);
            for (size_t i = 0; i < potionIdx.size() && i < 8; ++i) {
                int idx = potionIdx[i];
                auto* potion = static_cast<Potion*>(items[idx].get());
                std::string entry = "[" + std::to_string(i + 1) + "] " + potion->getName()
                    + " - " + potion->getDescription();
                renderer.drawStringBounded(8, 8 + static_cast<int>(i), 73, entry, Color::White);
            }
            renderer.drawStringBounded(8, 8 + static_cast<int>(std::min(potionIdx.size(), size_t(8))) + 1, 73,
                "按数字键选择，[Esc] 返回", Color::DarkGray);
            renderer.present();

            int itemKey = renderer.waitForKey();
            int itemIdx = itemKey - '1';
            if (itemIdx >= 0 && static_cast<size_t>(itemIdx) < potionIdx.size()) {
                int actualIdx = potionIdx[itemIdx];
                auto* potion = static_cast<Potion*>(items[actualIdx].get());
                std::string msg;

                switch (potion->getPotionType()) {
                    case PotionType::HealthPotion:
                        m_player->heal(potion->getHealAmount());
                        msg = "使用生命药水！恢复了 " + std::to_string(potion->getHealAmount()) + " 点生命。";
                        break;
                    case PotionType::EnergyDrink:
                        m_player->restoreEnergy(potion->getEnergyAmount());
                        msg = "使用能量饮料！恢复了 " + std::to_string(potion->getEnergyAmount()) + " 点能量。";
                        break;
                    case PotionType::PowerPotion:
                        m_player->applyBuff(potion->getAtkBonus(), 0, potion->getDuration());
                        msg = "使用力量药剂！ATK +" + std::to_string(potion->getAtkBonus())
                            + "，持续 " + std::to_string(potion->getDuration()) + " 回合。";
                        break;
                    case PotionType::ShieldScroll:
                        m_player->applyBuff(0, potion->getDefBonus(), potion->getDuration());
                        msg = "使用护盾卷轴！DEF +" + std::to_string(potion->getDefBonus())
                            + "，持续 " + std::to_string(potion->getDuration()) + " 回合。";
                        break;
                    case PotionType::ExperienceTome: {
                        std::mt19937 rng(std::random_device{}());  // 先创建具名变量
                        std::uniform_int_distribution<int> dist(0, 50);
                        int xpGain = 50 + dist(rng);
                        m_player->addXp(xpGain);
                        msg = "阅读经验之书！获得 " + std::to_string(xpGain) + " 点经验。";
                        break;
                    }
                    case PotionType::PurificationPotion:
                        m_player->clearBuffs();
                        msg = "使用净化药剂！清除了所有临时效果。";
                        break;
                    case PotionType::TeleportScroll:
                        msg = "传送卷轴不能在战斗中使用！";
                        // 不消耗物品，不跳过回合
                        m_battleLog.push_back(msg);
                        return;
                }
                m_battleLog.push_back(msg);
                inv.removeItem(actualIdx);
                processEnemyTurn();
                checkCombatEnd();
            }
        }
    }
    else if (key == 'c' || key == 'C') {
        // Enter forge UI from combat
        engine.runForgeUI();
    }
    else if (key == '5' || key == 'f' || key == 'F') {
        auto cmd = std::make_unique<FleeCommand>();
        std::string msg = cmd->execute(*m_player, *m_enemy);
        m_battleLog.push_back(msg);
        auto* fleeCmd = static_cast<FleeCommand*>(cmd.get());
        if (fleeCmd->succeeded()) {
            m_result = CombatResult::PlayerFled;
            m_player->addFleeCount();
            if (m_player->getFleeCount() >= 5) {
                // Infinite Loop ending triggered mid-game
                m_battleLog.push_back("while(true) { runAway(); }");
                EventBus::instance().publish(GameEvent::CombatEnded);
                EventBus::instance().publish(GameEvent::GameOver);
                return;
            }
            EventBus::instance().publish(GameEvent::CombatEnded);
        } else {
            pushCommand(std::move(cmd));
            processEnemyTurn();
            checkCombatEnd();
        }
    }
    else if (key == 'u' || key == 'U') {
        if (canUndo()) {
            std::string msg = undoLastCommand();
            m_battleLog.push_back(msg);
        } else {
            m_battleLog.push_back("没有可撤销的操作。");
        }
    }
    else if (key == 27) {
        m_result = CombatResult::PlayerLost;
        EventBus::instance().publish(GameEvent::CombatEnded);
    }
}

void CombatSystem::processEnemyTurn() {
    if (!m_enemy || !m_player) return;
    if (!m_enemy->isAlive()) return;

    ++m_turnCount;

    auto action = m_enemy->decideAction(*m_player);
    std::string msg;

    switch (action) {
        case Enemy::Action::Attack: {
            int dmg = m_player->takeDamage(m_enemy->dealDamage());
            msg = m_enemy->getName() + " 攻击了你，造成 " + std::to_string(dmg) + " 点伤害。";
            // Memory Leak 的特殊技能：额外光环伤害
            if (m_enemy->getEnemyType() == EnemyType::MemoryLeak) {
                int leakDmg = m_enemy->useSpecialSkill();
                m_player->takeDamage(leakDmg);
                msg += " [内存泄漏: " + std::to_string(leakDmg) + " 点额外伤害]";
            }
            break;
        }
        case Enemy::Action::Defend: {
            msg = m_enemy->getName() + " 进入防御姿态，本回合伤害减半。";
            // 增加临时防御
            break;
        }
        case Enemy::Action::Special: {
            int specialDmg = m_enemy->useSpecialSkill();
            if (specialDmg > 0) {
                int dmg = m_player->takeDamage(specialDmg);
                msg = m_enemy->getName() + " 使用了特殊技能！造成 " + std::to_string(dmg) + " 点伤害。";
            } else {
                msg = m_enemy->getName() + " 使用了特殊技能。";
            }
            break;
        }
        default:
            break;
    }

    m_battleLog.push_back(msg);

    // 递减玩家 Buff 回合数
    m_player->decrementBuffs();

    checkCombatEnd();
}

void CombatSystem::checkCombatEnd() {
    if (!m_player || !m_enemy) return;

    if (!m_enemy->isAlive()) {
        m_result = CombatResult::PlayerWin;
        m_battleLog.push_back(m_enemy->getName() + " 被击败！");

        // 经验值
        int xp = m_enemy->getXpReward();
        m_player->addXp(xp);
        m_battleLog.push_back("获得 " + std::to_string(xp) + " 点经验！");

        auto& inv = m_player->getInventory();
        std::mt19937 rng(std::random_device{}());

        // === 钥匙掉落（只有被标记的敌人才掉） ===
        if (m_enemy->isKeyHolder()) {
            if (m_enemy->isGuardian()) {
                // 守护兽掉落隐藏钥匙
                bool added = inv.addItem(std::make_unique<KeyItem>("隐藏钥匙",
                    "一把神秘的钥匙，似乎能打开隐藏的门"));
                if (added) {
                    m_battleLog.push_back("掉落: 隐藏钥匙!");
                } else {
                    m_battleLog.push_back("背包已满，隐藏钥匙无法拾取！");
                }
            } else {
                std::string keyName = (m_enemy->getEnemyType() == EnemyType::Segfault)
                    ? "主密钥" : "生锈的钥匙";
                auto key = std::make_unique<KeyItem>(keyName,
                    keyName + " - 解锁本层楼梯");
                bool added = inv.addItem(std::move(key));
                if (added) {
                    m_battleLog.push_back("获得: " + keyName + "!");
                } else {
                    m_battleLog.push_back("背包已满，" + keyName + "无法拾取！");
                }
            }
        }

        // === 任务物品掉落 ===
        std::string questDrop;
        switch (m_enemy->getEnemyType()) {
            case EnemyType::NullPointer:
                if (std::uniform_int_distribution<int>(1, 100)(rng) <= 60) {
                    auto qi = std::make_unique<QuestItem>(
                        "空指针分析报告", "一位学者关于空指针异常的笔记", 2);
                    inv.addItem(std::move(qi));
                    questDrop = "空指针分析报告";
                }
                break;
            case EnemyType::MemoryLeak:
                if (std::uniform_int_distribution<int>(1, 100)(rng) <= 60) {
                    auto qi = std::make_unique<QuestItem>(
                        "内存泄漏检测器", "一个在损坏内存附近会发出蜂鸣的装置", 3);
                    inv.addItem(std::move(qi));
                    questDrop = "内存泄漏检测器";
                }
                break;
            case EnemyType::StackOverflow:
                if (std::uniform_int_distribution<int>(1, 100)(rng) <= 40) {
                    auto qi = std::make_unique<QuestItem>(
                        "开发者日志残页", "一张来自开发者日志的残破页面", 4);
                    inv.addItem(std::move(qi));
                    questDrop = "开发者日志残页";
                    m_player->addGhostPage();
                }
                break;
            default: break;
        }
        if (!questDrop.empty())
            m_battleLog.push_back("敌人掉落: " + questDrop + "!");

        // === 药水掉落（按敌人类型概率） ===
        int potionRoll = std::uniform_int_distribution<int>(1, 100)(rng);
        switch (m_enemy->getEnemyType()) {
            case EnemyType::Bug:
                if (potionRoll <= 60) {
                    inv.addItem(std::make_unique<Potion>(PotionType::HealthPotion));
                    m_battleLog.push_back("掉落: 生命药水");
                } else if (potionRoll <= 100) {
                    inv.addItem(std::make_unique<Potion>(PotionType::EnergyDrink));
                    m_battleLog.push_back("掉落: 能量饮料");
                }
                break;
            case EnemyType::NullPointer:
                if (potionRoll <= 40) {
                    inv.addItem(std::make_unique<Potion>(PotionType::HealthPotion));
                    m_battleLog.push_back("掉落: 生命药水");
                } else if (potionRoll <= 70) {
                    inv.addItem(std::make_unique<Potion>(PotionType::EnergyDrink));
                    m_battleLog.push_back("掉落: 能量饮料");
                } else if (potionRoll <= 100) {
                    inv.addItem(std::make_unique<Potion>(PotionType::ShieldScroll));
                    m_battleLog.push_back("掉落: 护盾卷轴");
                }
                break;
            case EnemyType::MemoryLeak:
                if (potionRoll <= 30) {
                    inv.addItem(std::make_unique<Potion>(PotionType::HealthPotion));
                    m_battleLog.push_back("掉落: 生命药水");
                } else if (potionRoll <= 80) {
                    inv.addItem(std::make_unique<Potion>(PotionType::EnergyDrink));
                    m_battleLog.push_back("掉落: 能量饮料");
                } else if (potionRoll <= 100) {
                    inv.addItem(std::make_unique<Potion>(PotionType::PurificationPotion));
                    m_battleLog.push_back("掉落: 净化药剂");
                }
                break;
            case EnemyType::StackOverflow:
                if (potionRoll <= 30) {
                    inv.addItem(std::make_unique<Potion>(PotionType::HealthPotion));
                    m_battleLog.push_back("掉落: 生命药水");
                } else if (potionRoll <= 60) {
                    inv.addItem(std::make_unique<Potion>(PotionType::PowerPotion));
                    m_battleLog.push_back("掉落: 力量药剂");
                } else if (potionRoll <= 100) {
                    inv.addItem(std::make_unique<Potion>(PotionType::ShieldScroll));
                    m_battleLog.push_back("掉落: 护盾卷轴");
                }
                // 15% 概率掉落隐藏钥匙
                if (std::uniform_int_distribution<int>(1, 100)(rng) <= 15) {
                    bool added = inv.addItem(std::make_unique<KeyItem>("隐藏钥匙",
                        "一把神秘的钥匙，似乎能打开隐藏的门"));
                    if (added) {
                        m_battleLog.push_back("掉落: 隐藏钥匙!");
                    } else {
                        m_battleLog.push_back("背包已满，隐藏钥匙无法拾取！");
                    }
                }
                break;
            case EnemyType::Segfault:
                // Boss: 满血满蓝恢复
                m_player->heal(m_player->getMaxHp());
                m_player->restoreEnergy(m_player->getMaxEnergy());
                m_battleLog.push_back("Boss 击败！生命和能量完全恢复！");
                break;
            default: break;
        }

        // === 装备掉落 ===
        int equipChance = 0;
        Rarity equipMaxRarity = Rarity::Common;
        switch (m_enemy->getEnemyType()) {
            case EnemyType::Bug:
                equipChance = 10; equipMaxRarity = Rarity::Common; break;
            case EnemyType::NullPointer:
                equipChance = 20; equipMaxRarity = Rarity::Uncommon; break;
            case EnemyType::MemoryLeak:
                equipChance = 15; equipMaxRarity = Rarity::Rare; break;
            case EnemyType::StackOverflow:
                equipChance = 30; equipMaxRarity = Rarity::Rare; break;
            case EnemyType::Segfault:
                equipChance = 100; equipMaxRarity = Rarity::Legendary; break;
            default: break;
        }

        if (std::uniform_int_distribution<int>(1, 100)(rng) <= equipChance) {
            // 随机选择装备槽位和稀有度
            EquipSlot slot = static_cast<EquipSlot>(
                std::uniform_int_distribution<int>(0, 2)(rng));
            // 稀有度在 Common ~ equipMaxRarity 之间
            Rarity rarities[] = {Rarity::Common, Rarity::Uncommon, Rarity::Rare, Rarity::Legendary};
            int maxRarityIdx = static_cast<int>(equipMaxRarity);
            Rarity rarity = rarities[std::uniform_int_distribution<int>(0, maxRarityIdx)(rng)];

            std::string eqName, eqDesc;
            int atk = 0, def = 0, hp = 0;

            if (slot == EquipSlot::Weapon) {
                switch (rarity) {
                    case Rarity::Common:
                        eqName = "机械键盘"; atk = 5; eqDesc = "一把普通的机械键盘";
                        break;
                    case Rarity::Uncommon:
                        eqName = "Cherry MX 键盘"; atk = 10; eqDesc = "手感极佳的机械键盘";
                        break;
                    case Rarity::Rare:
                        eqName = "黑客键盘"; atk = 15; def = 3; eqDesc = "黑客专用的定制键盘";
                        break;
                    case Rarity::Legendary:
                        eqName = "传奇 IDE 之剑"; atk = 25; hp = 30; eqDesc = "以纯粹代码锻造的神剑";
                        break;
                }
            } else if (slot == EquipSlot::Armor) {
                switch (rarity) {
                    case Rarity::Common:
                        eqName = "防静电手环"; def = 3; eqDesc = "防止静电损坏元件";
                        break;
                    case Rarity::Uncommon:
                        eqName = "代码审查盾"; def = 8; hp = 10; eqDesc = "经过严格审查的防御";
                        break;
                    case Rarity::Rare:
                        eqName = "防火墙铠甲"; def = 15; eqDesc = "坚不可摧的网络防线";
                        break;
                    case Rarity::Legendary:
                        eqName = "开源者斗篷"; def = 20; hp = 50; eqDesc = "由社区贡献者编织的传奇斗篷";
                        break;
                }
            } else {
                switch (rarity) {
                    case Rarity::Common:
                        eqName = "咖啡杯"; hp = 10; eqDesc = "程序员的最爱，提神醒脑";
                        break;
                    case Rarity::Uncommon:
                        eqName = "橡皮鸭"; atk = 3; def = 3; eqDesc = "对着它解释代码就能找到 bug";
                        break;
                    case Rarity::Rare:
                        eqName = "固态硬盘 SSD"; atk = 5; def = 5; hp = 15; eqDesc = "闪电般的读写速度";
                        break;
                    case Rarity::Legendary:
                        eqName = "量子处理器"; atk = 10; def = 10; hp = 40; eqDesc = "超越经典计算的存在";
                        break;
                }
            }

            std::string rarityStr;
            switch (rarity) {
                case Rarity::Common: rarityStr = "普通"; break;
                case Rarity::Uncommon: rarityStr = "稀有"; break;
                case Rarity::Rare: rarityStr = "极稀有"; break;
                case Rarity::Legendary: rarityStr = "传说"; break;
            }

            auto eq = std::make_unique<Equipment>(eqName, slot, rarity, atk, def, hp, eqDesc);
            inv.addItem(std::move(eq));
            m_battleLog.push_back("掉落装备: [" + rarityStr + "] " + eqName + "!");
        }

        // === 碎片掉落（大幅增加） ===
        int fragCount = 1;
        Rarity fragRarity = Rarity::Common;
        switch (m_enemy->getEnemyType()) {
            case EnemyType::Bug:
                fragCount = std::uniform_int_distribution<int>(3, 5)(rng); fragRarity = Rarity::Common; break;
            case EnemyType::NullPointer:
                fragCount = std::uniform_int_distribution<int>(3, 5)(rng); fragRarity = Rarity::Common; break;
            case EnemyType::MemoryLeak:
                fragCount = std::uniform_int_distribution<int>(3, 5)(rng); fragRarity = Rarity::Common; break;
            case EnemyType::StackOverflow:
                fragCount = std::uniform_int_distribution<int>(4, 6)(rng); fragRarity = Rarity::Uncommon; break;
            case EnemyType::Segfault:
                fragCount = std::uniform_int_distribution<int>(5, 8)(rng); fragRarity = Rarity::Rare; break;
            default: break;
        }

        static const std::vector<std::string> commonFrags = {
            "for", "while", "if", "else", "int", "void", "bool",
            "return", "break", "const", "+", "-", "Attack", "Defend", "Heal"
        };
        static const std::vector<std::string> uncommonFrags = {
            "class", "new", "delete", "this", "try", "catch", "virtual",
            "switch", "case", "throw", "char*"
        };
        static const std::vector<std::string> rareFrags = {
            "template", "Counter", "virtual", "throw", "goto", "namespace"
        };

        int fragsDropped = 0;
        for (int i = 0; i < fragCount; ++i) {
            const auto& pool = (fragRarity == Rarity::Rare) ? rareFrags :
                               (fragRarity == Rarity::Uncommon) ? uncommonFrags : commonFrags;
            std::string kw = pool[std::uniform_int_distribution<int>(0,
                static_cast<int>(pool.size()) - 1)(rng)];
            FragmentCategory cat = CodeFragment::categoryFromKeyword(kw);
            inv.addItem(std::make_unique<CodeFragment>(kw, cat, fragRarity, kw + " 碎片"));
            ++fragsDropped;
        }
        // Segfault 额外掉落 1 个传说碎片
        if (m_enemy->getEnemyType() == EnemyType::Segfault) {
            static const std::vector<std::string> legendaryFrags = {
                "virtual", "throw", "Counter", "template", "namespace", "goto"
            };
            std::string lkw = legendaryFrags[std::uniform_int_distribution<int>(0,
                static_cast<int>(legendaryFrags.size()) - 1)(rng)];
            FragmentCategory lcat = CodeFragment::categoryFromKeyword(lkw);
            inv.addItem(std::make_unique<CodeFragment>(lkw, lcat, Rarity::Legendary,
                "传说碎片 (" + lkw + ")"));
            m_battleLog.push_back("掉落传说碎片: [" + lkw + "]!");
            ++fragsDropped;
        }

        if (fragsDropped > 0) {
            m_battleLog.push_back("获得 " + std::to_string(fragsDropped) + " 个碎片！");
        }

        EventBus::instance().publish(GameEvent::EnemyKilled, m_enemy);
        EventBus::instance().publish(GameEvent::CombatEnded);
    }
    else if (!m_player->isAlive()) {
        m_result = CombatResult::PlayerLost;
        m_battleLog.push_back("你被 " + m_enemy->getName() + " 击败了……");

        EventBus::instance().publish(GameEvent::CombatEnded);
        EventBus::instance().publish(GameEvent::GameOver);
    }
}

void CombatSystem::pushCommand(std::unique_ptr<CombatCommand> cmd) {
    m_commandHistory.push(std::move(cmd));
}

std::string CombatSystem::undoLastCommand() {
    if (m_commandHistory.empty()) {
        return "没有可撤销的操作。";
    }

    auto cmd = std::move(m_commandHistory.top());
    m_commandHistory.pop();

    // 撤销 = 恢复玩家回合 + 敌人的回合
    // 简化：撤销一次玩家命令，回退两方状态
    return cmd->undo(*m_player, *m_enemy);
}
