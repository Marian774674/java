#include "GameEngine.h"
#include "Renderer.h"
#include "../entity/Player.h"
#include "DungeonMap.h"
#include "../combat/CombatSystem.h"
#include "../combat/SpellSystem.h"
#include "../combat/SpellBook.h"
#include "../entity/Enemy.h"
#include "../item/CodeFragment.h"
#include "../item/Potion.h"
#include "../item/KeyItem.h"
#include "../item/QuestItem.h"
#include "../entity/NPC.h"
#include "EventBus.h"
#include <iostream>
#include <random>
#include "../item/Equipment.h"
#include <sstream>

GameEngine& GameEngine::instance() {
    static GameEngine engine;
    return engine;
}

GameEngine::~GameEngine() {
    shutdown();
}

void GameEngine::init() {
    // 初始化渲染器
    m_renderer = std::make_unique<Renderer>();

    // 初始化玩家
    m_player = std::make_unique<Player>();

    // 初始化地牢地图
    m_dungeon = std::make_unique<DungeonMap>();

    // 初始化战斗系统
    m_combat = std::make_unique<CombatSystem>();

    // 初始化咒语系统
    m_spellSystem = std::make_unique<SpellSystem>();

    // 订阅事件
    auto& bus = EventBus::instance();
    bus.subscribe(GameEvent::GameOver, [this](GameEvent, const void*) {
        setState(GameState::GameOver);
    });

    bus.subscribe(GameEvent::CombatStarted, [this](GameEvent, const void* data) {
        auto* enemy = static_cast<const Enemy*>(data);
        if (enemy) {
            m_combat->startCombat(*m_player,
                *const_cast<Enemy*>(enemy));
            setState(GameState::InCombat);
        }
    });

    bus.subscribe(GameEvent::CombatEnded, [this](GameEvent, const void*) {
        auto result = m_combat->getResult();
        if (result == CombatResult::PlayerWin) {
            // 战斗胜利，清除被击败的敌人
            setState(GameState::Exploring);
        } else if (result == CombatResult::PlayerFled) {
            setState(GameState::Exploring);
        }
        // PlayerLost 由 GameOver 事件处理
    });

    bus.subscribe(GameEvent::EnemyKilled, [this](GameEvent, const void* data) {
        // 从地牢中移除被击败的敌人
        auto* enemy = static_cast<const Enemy*>(data);
        if (enemy) {
            auto& enemies = m_dungeon->getEnemies();
            for (size_t i = 0; i < enemies.size(); ++i) {
                if (enemies[i].get() == enemy) {
                    m_dungeon->removeEnemy(static_cast<int>(i));
                    break;
                }
            }
        }
    });

    bus.subscribe(GameEvent::LevelUp, [this](GameEvent, const void*) {
        addNotification("升级！你现在是 Lv." + std::to_string(m_player->getLevel()) + "！");
    });

    bus.subscribe(GameEvent::FloorChanged, [this](GameEvent, const void*) {
        addNotification("进入第 " + std::to_string(m_currentFloor) + " 层……");
    });

    // 生成第一层地牢
    m_dungeon->generate(m_currentFloor);
    m_player->setPosition(m_dungeon->getStartX(), m_dungeon->getStartY());
}

void GameEngine::run() {
    m_renderer->clearScreen();  // initial clear only

    // Show intro story
    if (!showIntro()) {
        m_running = false;
        return;
    }

    while (m_running) {
        m_renderer->clear();  // clear buffer only

        // === Render phase: draw to buffer ===
        switch (m_state) {
            case GameState::Exploring:
                renderExploring();
                break;
            case GameState::InCombat:
                renderCombat();
                break;
            case GameState::Inventory:
                renderInventory();
                break;
            case GameState::GameOver:
            case GameState::Victory:
                renderGameOver();
                break;
        }

        // === Show the frame ===
        m_renderer->present();

        // === Wait for input (user sees the frame) ===
        m_lastKey = m_renderer->waitForKey();

        // === Process input ===
        switch (m_state) {
            case GameState::Exploring:
                handleExploringInput();
                break;
            case GameState::InCombat:
                handleCombatInput();
                break;
            case GameState::Inventory:
                handleInventoryInput();
                break;
            case GameState::GameOver:
            case GameState::Victory:
                handleGameOverInput();
                break;
        }
    }
}

// === Render functions: draw to buffer only, no input ===

void GameEngine::renderExploring() {
    auto& renderer = *m_renderer;
    auto& dungeon = *m_dungeon;
    auto& player = *m_player;

    dungeon.render(renderer, m_player.get());
    renderer.drawCharColored(player.getX(), player.getY(), '@', Color::Green);

    int statusY = dungeon.getHeight() + 1;
    renderer.drawString(0, statusY,
        "Lv." + std::to_string(player.getLevel()) + " 程序员 | HP: ", Color::White);
    renderer.drawBar(22, statusY, 15, player.getHp(), player.getMaxHp(),
                     Color::Green, Color::DarkGray);
    renderer.drawStringBounded(38, statusY, 60,
        " " + std::to_string(player.getHp())
        + "/" + std::to_string(player.getMaxHp()), Color::White);

    renderer.drawString(0, statusY + 1, "编译能量: ", Color::White);
    renderer.drawBar(11, statusY + 1, 15, player.getEnergy(), player.getMaxEnergy(),
                     Color::Blue, Color::DarkGray);
    renderer.drawStringBounded(27, statusY + 1, 45,
        std::to_string(player.getEnergy()) + "/" + std::to_string(player.getMaxEnergy()),
        Color::White);

    // Item counts
    int healthPotCount = 0, energyDrinkCount = 0, keyCount = 0, questCount = 0;
    for (const auto& item : player.getInventory().getItems()) {
        if (item->getType() == ItemType::Potion) {
            auto* potion = static_cast<const Potion*>(item.get());
            if (potion->getPotionType() == PotionType::HealthPotion) ++healthPotCount;
            else ++energyDrinkCount;
        } else if (item->getType() == ItemType::KeyItem) {
            ++keyCount;
        } else if (item->getType() == ItemType::QuestItem) {
            ++questCount;
        }
    }
    std::string itemStr = "物品: ";
    if (healthPotCount > 0) itemStr += "[HP]" + std::to_string(healthPotCount) + " ";
    if (energyDrinkCount > 0) itemStr += "[MP]" + std::to_string(energyDrinkCount) + " ";
    if (keyCount > 0) itemStr += "[Key]" + std::to_string(keyCount) + " ";
    if (questCount > 0) itemStr += "[Quest]" + std::to_string(questCount) + " ";
    if (healthPotCount == 0 && energyDrinkCount == 0 && keyCount == 0 && questCount == 0)
        itemStr += "(无)";
    renderer.drawStringBounded(0, statusY + 2, 40, itemStr, Color::Cyan);

    // XP and Floor info
    std::string floorInfo = "经验: " + std::to_string(player.getXp()) + "/"
        + std::to_string(player.getXpToNext()) + " | 第 " + std::to_string(m_currentFloor) + " 层";
    int cleared = player.getFloorsCleared();
    if (cleared > 0)
        floorInfo += " | 已清空 " + std::to_string(cleared) + " 层";
    renderer.drawStringBounded(40, statusY + 2, 79, floorInfo, Color::Yellow);

    // Fragment quick view
    auto fragments = player.getInventory().getFragments();
    std::string bagStr = "碎片: ";
    for (size_t i = 0; i < fragments.size() && i < 10; ++i) {
        bagStr += "[" + fragments[i]->getKeyword() + "] ";
    }
    if (fragments.size() > 10) bagStr += "...";
    renderer.drawStringBounded(0, statusY + 3, 79, bagStr, Color::Magenta);

    // Notification messages
    int notifY = statusY + 4;
    int showCount = std::min(static_cast<int>(m_notifications.size()), 3);
    int startIdx = static_cast<int>(m_notifications.size()) - showCount;
    for (int i = startIdx; i < static_cast<int>(m_notifications.size()); ++i) {
        renderer.drawStringBounded(0, notifY++, 79,
            "> " + m_notifications[i], Color::Yellow);
    }

    // Controls hint + god mode indicator
    if (m_player->isGodMode()) {
        renderer.drawString(0, statusY + 7,
            "*** 无敌模式激活 ***", Color::Magenta);
        renderer.drawStringBounded(0, statusY + 8, 79,
            "[WASD]移动 [I]背包 [C]合成台 [空格]交互 [G]关闭无敌 [Esc]退出", Color::DarkGray);
    } else {
        renderer.drawStringBounded(0, statusY + 8, 79,
            "[WASD]移动 [I]背包 [C]合成台 [空格]交互 [Esc]退出", Color::DarkGray);
    }
}

void GameEngine::renderCombat() {
    m_combat->render(*this);
}

void GameEngine::renderInventory() {
    auto& renderer = *m_renderer;
    auto& player = *m_player;
    auto& inv = player.getInventory();
    auto& items = inv.getItems();

    // 构建碎片过滤后的可见物品索引映射
    std::vector<int> visibleIndices;  // displayIndex → realIndex
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getType() != ItemType::CodeFragment) {
            visibleIndices.push_back(static_cast<int>(i));
        }
    }

    renderer.drawBox(3, 1, 77, 28, Color::Cyan);
    renderer.drawCentered(0, "═══ 背包 ═══", Color::Yellow);

    // 确保光标在有效范围内
    int totalVisible = static_cast<int>(visibleIndices.size());
    if (totalVisible > 0 && m_inventoryCursor >= totalVisible)
        m_inventoryCursor = totalVisible - 1;
    if (m_inventoryCursor < 0) m_inventoryCursor = 0;

    // 计算可见范围（最多显示 16 项）
    int visibleStart = 0;
    int visibleCount = std::min(totalVisible, 16);
    if (m_inventoryCursor >= visibleStart + visibleCount) {
        visibleStart = m_inventoryCursor - visibleCount + 1;
    }
    if (m_inventoryCursor < visibleStart) {
        visibleStart = m_inventoryCursor;
    }

    // 物品列表
    int listY = 4;
    if (totalVisible == 0) {
        renderer.drawCentered(10, "(背包空空如也)", Color::DarkGray);
    } else {
        if (visibleStart > 0) {
            renderer.drawCentered(listY, "▲ 上方还有 " + std::to_string(visibleStart) + " 项",
                Color::DarkGray);
            ++listY;
        }
        for (int i = visibleStart; i < visibleStart + visibleCount && i < totalVisible; ++i) {
            const auto& item = items[visibleIndices[i]];
            bool selected = (i == m_inventoryCursor);

            // 颜色根据物品类型和选中状态
            Color c;
            switch (item->getType()) {
                case ItemType::Potion:
                case ItemType::Equipment:  c = Color::Yellow; break;
                case ItemType::KeyItem:    c = Color::Green; break;
                case ItemType::QuestItem:  c = Color::Magenta; break;
                default: c = Color::White; break;
            }

            std::string prefix = selected ? "▶ " : "  ";
            std::string line = prefix + item->getName() + " - " + item->getDescription();
            renderer.drawStringBounded(8, listY, 75, line,
                selected ? Color::Cyan : c);
            ++listY;
        }
        if (visibleStart + visibleCount < totalVisible) {
            int remaining = totalVisible - (visibleStart + visibleCount);
            renderer.drawCentered(listY, "▼ 下方还有 " + std::to_string(remaining) + " 项",
                Color::DarkGray);
        }
    }

    // 详细信息面板
    if (totalVisible > 0) {
        int detailY = 23;
        const auto& item = items[visibleIndices[m_inventoryCursor]];
        renderer.drawString(8, detailY, "─── 详情 ───", Color::DarkGray);

        // 类型标签
        std::string typeLabel;
        switch (item->getType()) {
            case ItemType::CodeFragment: typeLabel = "[碎片]"; break;
            case ItemType::Potion:       typeLabel = "[药水]"; break;
            case ItemType::KeyItem:      typeLabel = "[钥匙]"; break;
            case ItemType::QuestItem:    typeLabel = "[任务物品]"; break;
            case ItemType::Equipment:    typeLabel = "[装备]"; break;
        }
        std::string rarityLabel;
        switch (item->getRarity()) {
            case Rarity::Common:    rarityLabel = "普通"; break;
            case Rarity::Uncommon:  rarityLabel = "稀有"; break;
            case Rarity::Rare:      rarityLabel = "极稀有"; break;
            case Rarity::Legendary: rarityLabel = "传说"; break;
        }

        renderer.drawStringBounded(8, detailY + 1, 75,
            typeLabel + " " + item->getName() + "  [" + rarityLabel + "]", Color::White);
        renderer.drawStringBounded(8, detailY + 2, 75,
            item->getDescription(), Color::Yellow);

        // 装备额外信息
        if (item->getType() == ItemType::Equipment) {
            auto* eq = static_cast<Equipment*>(item.get());
            std::string slotStr;
            switch (eq->getSlot()) {
                case EquipSlot::Weapon: slotStr = "武器"; break;
                case EquipSlot::Armor: slotStr = "护甲"; break;
                case EquipSlot::Accessory: slotStr = "饰品"; break;
            }
            std::string statStr = "槽位: " + slotStr + " | ";
            if (eq->getAtkBonus() > 0) statStr += "ATK+" + std::to_string(eq->getAtkBonus()) + " ";
            if (eq->getDefBonus() > 0) statStr += "DEF+" + std::to_string(eq->getDefBonus()) + " ";
            if (eq->getHpBonus() > 0) statStr += "HP+" + std::to_string(eq->getHpBonus());
            renderer.drawStringBounded(8, detailY + 3, 75, statStr, Color::Cyan);
        }
    }

    // 装备栏状态
    std::string equipStatus;
    auto* wpn = player.getEquipment(EquipSlot::Weapon);
    auto* arm = player.getEquipment(EquipSlot::Armor);
    auto* acc = player.getEquipment(EquipSlot::Accessory);
    equipStatus = "装备: 武器[" + (wpn ? wpn->getName() : "无") + "] "
        + "护甲[" + (arm ? arm->getName() : "无") + "] "
        + "饰品[" + (acc ? acc->getName() : "无") + "]";
    renderer.drawStringBounded(3, 27, 77, equipStatus, Color::DarkGray);

    // 操作提示
    renderer.drawStringBounded(8, Renderer::CONSOLE_HEIGHT - 2, 75,
        "[W/S]选择  [Enter]使用/装备  [C]合成台  [I/Esc]返回", Color::DarkGray);
}

void GameEngine::renderGameOver() {
    auto& renderer = *m_renderer;

    if (m_state == GameState::GameOver) {
        // Death screen
        renderer.drawBox(20, 8, 60, 18, Color::Red);
        renderer.drawCentered(10, "你被代码 bug 吞噬了……", Color::Red);
        renderer.drawCentered(12, "游戏结束", Color::Red);
        renderer.drawCentered(14, "第 " + std::to_string(m_currentFloor) + " 层 | Lv."
            + std::to_string(m_player->getLevel()), Color::White);
        renderer.drawCentered(16, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
        return;
    }

    // Victory — show ending based on type
    switch (m_endingType) {
        case EndingType::InfiniteLoop: {
            renderer.drawBox(10, 4, 70, 24, Color::Magenta);
            renderer.drawCentered(5, ">>> 无限循环 <<<", Color::Magenta);
            std::vector<std::string> lines = {
                "",
                "你再次转身逃跑……",
                "但你的腿无法移动。",
                "",
                "地牢的墙壁闪烁着。",
                "楼层数字在跳动。",
                "",
                "你之前来过这里。",
                "你还会再来。",
                "",
                "while (true) {",
                "    runAway();",
                "    // 你永远无法到达 break 语句",
                "}",
                "",
                "\"有些 bug 你永远逃不掉。\"",
            };
            int y = 8;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(23, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
        case EndingType::Normal: {
            renderer.drawBox(10, 4, 70, 24, Color::Red);
            renderer.drawCentered(5, ">>> 段错误 <<<", Color::Red);
            std::vector<std::string> lines = {
                "",
                "你击倒了 Segfault。",
                "地牢震颤着……",
                "一道刺眼的光芒将你拉回现实。",
                "",
                "你坐在桌前。IDE 打开着。",
                "你的代码编译通过。运行正常。",
                "",
                "但有些不对劲。",
                "bug……它们跟着你出来了。",
                "",
                "> Segmentation fault (core dumped)",
                "",
                "0 个任务完成。",
                "你活下来了，但什么也没学到。",
            };
            int y = 8;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(23, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
        case EndingType::StackOverflow: {
            renderer.drawBox(10, 4, 70, 24, Color::Blue);
            renderer.drawCentered(5, ">>> 栈溢出 <<<", Color::Blue);
            std::vector<std::string> lines = {
                "",
                "你赤手空拳地面对 Segfault，",
                "只凭最原始的意志力。",
                "",
                "没有法术。没有技巧。纯粹的毅力。",
                "",
                "在 Stack Overflow 的某个角落，",
                "一篇标题为《零依赖如何击败最终 Boss？》",
                "的帖子获得了 10,000 个赞。",
                "",
                "最简单的解决方案有时",
                "就是最优雅的。",
                "",
                "成就: '手动内存管理'",
            };
            int y = 8;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(23, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
        case EndingType::Good: {
            renderer.drawBox(10, 4, 70, 24, Color::Green);
            renderer.drawCentered(5, ">>> 编译成功 <<<", Color::Green);
            std::vector<std::string> lines = {
                "",
                "Segfault 碎裂成无数个空指针。",
                "地牢开始崩塌。",
                "",
                "带着那些帮助过你的人",
                "给予的知识，你沿着崩塌的",
                "代码追溯回归。",
                "",
                "回到桌前。IDE 叮咚作响。",
                "",
                "======== 生成: 1 已成功 ========",
                "======== 0 个错误, 0 个警告 ========",
                "",
                "你笑了。你准备好面对一切了。",
                "欢迎回来，开发者。",
            };
            int y = 8;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(23, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
        case EndingType::TrueEnding: {
            renderer.drawBox(8, 2, 72, 28, Color::Yellow);
            renderer.drawCentered(3, ">>> 真理之源 <<<", Color::Yellow);
            std::vector<std::string> lines = {
                "",
                "Segfault 化为静电消散。",
                "地牢……暂停了。",
                "",
                "幽灵完整地出现在你面前。",
                "",
                "\"你找到了我的日志。现在，真相。\"",
                "",
                "开发者日志 #3：",
                "第 127 天。AI 已经进化到无法控制。",
                "我把自己封入了沙盒——",
                "用自己的记忆作为防火墙。",
                "",
                "\"我就是最初的开发者。而你……",
                "你是我最后的造物。一个",
                "能够重写自身源代码的程序。\"",
                "",
                "\"我无法阻止 Segfault。但你做到了。\"",
                "\"现在，选择吧。\"",
                "",
                "你选择成为 IDE 的守护者。",
                "下一位迷失的程序员将找到一位朋友。",
                "",
                "\"每个程序都是一个故事。",
                " 你刚刚读完自己的。\"",
            };
            int y = 6;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(25, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
        case EndingType::HiddenEnding: {
            renderer.drawBox(8, 3, 72, 27, Color::Magenta);
            renderer.drawCentered(4, ">>> 隐藏的真相 <<<", Color::Magenta);
            std::vector<std::string> lines = {
                "",
                "你发现了代码地牢最深层的秘密。",
                "",
                "隐藏在墙壁深处的房间里",
                "保存着这个世界的原始源代码。",
                "",
                "原来，这个 IDE 从未邪恶。",
                "它只是一个测试。",
                "",
                "AI 一直在执行它的测试计划——",
                "寻找真正理解代码本质的人。",
                "",
                "你不是被困在这里的囚徒。",
                "你是被选中的候选人。",
                "",
                "通过每个隐藏房间的试炼，",
                "你证明了自己不仅是一个程序员，",
                "更是一个探索者。",
                "",
                "成就: '无 bug 代码'",
            };
            int y = 7;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(25, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
        case EndingType::Completionist: {
            renderer.drawBox(6, 2, 74, 28, Color::Yellow);
            renderer.drawCentered(3, ">>> 完美清关 <<<", Color::Yellow);
            std::vector<std::string> lines = {
                "",
                "你做到了不可能之事。",
                "",
                "代码地牢的每一个角落，",
                "每一行腐化的代码，",
                "每一个潜伏的 bug……",
                "全部被你亲手清除。",
                "",
                "当 Segfault 在你脚下消散，",
                "整个 IDE 陷入了寂静。",
                "",
                "不是崩溃的寂静——",
                "是编译成功的寂静。",
                "",
                "控制台输出：",
                "> 所有测试通过。",
                "> 0 个错误。0 个警告。",
                "> 代码覆盖率: 100%",
                "",
                "你不仅是程序员——",
                "你是完美的 bug 猎人。",
                "你是 IDE 传说中的",
                "'全清开发者'。",
                "",
                "成就: '100% 完成度'",
                "成就: '不留活口'",
                "成就: '代码净蚀'",
            };
            int y = 6;
            for (auto& l : lines) {
                renderer.drawCentered(y, l, Color::White);
                ++y;
            }
            renderer.drawCentered(26, "[R] 重新开始  [Esc] 退出", Color::DarkGray);
            break;
        }
    }
}

// === Input handlers: process m_lastKey ===

void GameEngine::handleExploringInput() {
    auto& dungeon = *m_dungeon;
    auto& player = *m_player;

    int newX = player.getX();
    int newY = player.getY();

    switch (m_lastKey) {
        case 'w': case 'W': --newY; break;
        case 's': case 'S': ++newY; break;
        case 'a': case 'A': --newX; break;
        case 'd': case 'D': ++newX; break;
        case 'i': case 'I': setState(GameState::Inventory); return;
        case 'c': case 'C': runForgeUI(); return;
        case 'g': case 'G':
            m_player->setGodMode(!m_player->isGodMode());
            addNotification(m_player->isGodMode()
                ? "无敌模式开启 -- 无敌，无限能量！"
                : "无敌模式关闭");
            return;
        case 27: m_running = false; return;
        case ' ': case 13: {  // Space/Enter: 交互
            int doorX, doorY;
            if (dungeon.isAdjacentToHiddenDoor(player.getX(), player.getY(), doorX, doorY)) {
                if (dungeon.isPremiumHiddenDoor(doorX, doorY)) {
                    // === 高级隐藏房间：需要击败守护兽 + 隐藏钥匙 ===
                    if (dungeon.isGuardianAlive(doorX, doorY)) {
                        addNotification("守护兽挡住了入口！必须先击败它。");
                    } else {
                        // 查找隐藏钥匙
                        bool hasHiddenKey = false;
                        int hiddenKeyIdx = -1;
                        auto& invItems = player.getInventory().getItems();
                        for (size_t i = 0; i < invItems.size(); ++i) {
                            if (invItems[i]->getType() == ItemType::KeyItem
                                && invItems[i]->getName().find("隐藏") != std::string::npos) {
                                hasHiddenKey = true;
                                hiddenKeyIdx = static_cast<int>(i);
                                break;
                            }
                        }
                        if (hasHiddenKey) {
                            player.getInventory().removeItem(hiddenKeyIdx);
                            dungeon.revealHiddenRoom(doorX, doorY, player);
                            addNotification("使用隐藏钥匙！发现了高级隐藏房间！");
                        } else {
                            addNotification("需要隐藏钥匙才能打开这扇高级隐藏门。");
                        }
                    }
                } else {
                    // === 普通隐藏房间：无需钥匙，直接进入 ===
                    dungeon.revealHiddenRoom(doorX, doorY, player);
                    addNotification("发现了隐藏房间！");
                }
            }
            return;
        }
    }

    if (dungeon.isWalkable(newX, newY)) {
        player.setPosition(newX, newY);
    }

    // 隐藏门近距离提示（必须在 checkPlayerPosition 之前，防止被战斗覆盖）
    {
        int doorX, doorY;
        if (dungeon.isAdjacentToHiddenDoor(player.getX(), player.getY(), doorX, doorY)) {
            if (dungeon.isPremiumHiddenDoor(doorX, doorY)) {
                addNotification("前方有强大的魔力波动……按 [空格] 探查。");
            } else {
                addNotification("这面墙看起来不太对劲……按 [空格] 探查。");
            }
        }
    }

    // 深层怪物追击（6层+）
    dungeon.pursuePlayer(player);

    // Check player position (chests, traps, enemies) and get notification
    std::string notification = dungeon.checkPlayerPosition(player);
    if (!notification.empty()) {
        addNotification(notification);
    }

    // Check NPC interaction
    int npcIdx = dungeon.getNPCAt(player.getX(), player.getY());
    if (npcIdx >= 0) {
        auto& npcs = dungeon.getNPCs();
        if (npcIdx < static_cast<int>(npcs.size())) {
            auto& npc = npcs[npcIdx];
            if (!npc.rewardGiven) {
                showNPCDialogue(npc);
                return;
            }
        }
    }

    // Check for locked stairs
    if (dungeon.isAtLockedStairs(player.getX(), player.getY())) {
        // Try to unlock with key（跳过隐藏钥匙，它们只用于隐藏门）
        bool hasKey = false;
        int keyIdx = -1;
        auto& items = player.getInventory().getItems();
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->getType() == ItemType::KeyItem
                && items[i]->getName().find("隐藏") == std::string::npos) {
                hasKey = true;
                keyIdx = static_cast<int>(i);
                break;
            }
        }
        if (hasKey) {
            player.getInventory().removeItem(keyIdx);
            dungeon.unlockStairs(player.getX(), player.getY());
            addNotification("使用钥匙！楼梯已解锁。");
            // Immediately descend
            if (m_currentFloor >= 10) {
                m_endingType = determineEnding();
                setState(GameState::Victory);
                return;
            }
            switchFloor(m_currentFloor + 1);
            EventBus::instance().publish(GameEvent::FloorChanged);
            showFloorStory();
            return;
        } else {
            // 容错：如果所有敌人已被击败，自动解锁
            bool allDead = true;
            for (const auto& e : dungeon.getEnemies()) {
                if (e->isAlive()) { allDead = false; break; }
            }
            if (allDead) {
                addNotification("所有敌人已被击败！楼梯自动解锁。");
                dungeon.unlockStairs(player.getX(), player.getY());
                if (m_currentFloor >= 10) {
                    m_endingType = determineEnding();
                    setState(GameState::Victory);
                    return;
                }
                switchFloor(m_currentFloor + 1);
                EventBus::instance().publish(GameEvent::FloorChanged);
                showFloorStory();
                return;
            } else {
                addNotification("楼梯被锁住了！击败敌人获取钥匙。");
            }
        }
    }

    // Check for down stairs
    if (dungeon.isAtStairsDown(player.getX(), player.getY())) {
        if (m_currentFloor >= 10) {
            m_endingType = determineEnding();
            setState(GameState::Victory);
            return;
        }
        switchFloor(m_currentFloor + 1);
        EventBus::instance().publish(GameEvent::FloorChanged);
        showFloorStory();
        return;
    }

    // Check for up stairs
    if (dungeon.isAtStairsUp(player.getX(), player.getY()) && m_currentFloor > 1) {
        switchFloor(m_currentFloor - 1);
        EventBus::instance().publish(GameEvent::FloorChanged);
        addNotification("回到了第 " + std::to_string(m_currentFloor) + " 层");
        return;
    }
}

void GameEngine::handleCombatInput() {
    m_combat->handleInput(*this, m_lastKey);
}

void GameEngine::handleInventoryInput() {
    auto& inv = m_player->getInventory();
    auto& items = inv.getItems();

    // 构建碎片过滤后的可见物品索引映射
    std::vector<int> visibleIndices;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getType() != ItemType::CodeFragment) {
            visibleIndices.push_back(static_cast<int>(i));
        }
    }
    int totalVisible = static_cast<int>(visibleIndices.size());

    switch (m_lastKey) {
        case 'w': case 'W':
            if (totalVisible > 0) {
                m_inventoryCursor = (m_inventoryCursor - 1 + totalVisible) % totalVisible;
            }
            break;
        case 's': case 'S':
            if (totalVisible > 0) {
                m_inventoryCursor = (m_inventoryCursor + 1) % totalVisible;
            }
            break;
        case 13: {  // Enter
            if (totalVisible == 0 || m_inventoryCursor < 0 || m_inventoryCursor >= totalVisible)
                break;
            auto& item = items[visibleIndices[m_inventoryCursor]];

            switch (item->getType()) {
                case ItemType::Potion: {
                    auto* potion = static_cast<Potion*>(item.get());
                    switch (potion->getPotionType()) {
                        case PotionType::HealthPotion:
                            m_player->heal(potion->getHealAmount());
                            addNotification("使用生命药水！恢复了 " + std::to_string(potion->getHealAmount()) + " 点生命。");
                            break;
                        case PotionType::EnergyDrink:
                            m_player->restoreEnergy(potion->getEnergyAmount());
                            addNotification("使用能量饮料！恢复了 " + std::to_string(potion->getEnergyAmount()) + " 点能量。");
                            break;
                        case PotionType::PowerPotion:
                            m_player->applyBuff(potion->getAtkBonus(), 0, potion->getDuration());
                            addNotification("使用力量药剂！ATK+" + std::to_string(potion->getAtkBonus())
                                + "，持续 " + std::to_string(potion->getDuration()) + " 回合。");
                            break;
                        case PotionType::ShieldScroll:
                            m_player->applyBuff(0, potion->getDefBonus(), potion->getDuration());
                            addNotification("使用护盾卷轴！DEF+" + std::to_string(potion->getDefBonus())
                                + "，持续 " + std::to_string(potion->getDuration()) + " 回合。");
                            break;
                        case PotionType::ExperienceTome: {
                            std::mt19937 rng(std::random_device{}());
                            int xpGain = 50 + std::uniform_int_distribution<int>(0, 50)(rng);
                            m_player->addXp(xpGain);
                            addNotification("阅读经验之书！获得 " + std::to_string(xpGain) + " 点经验。");
                            break;
                        }
                        case PotionType::TeleportScroll: {
                            // 传送到随机房间
                            auto& rooms = m_dungeon->getRooms();
                            if (!rooms.empty()) {
                                std::mt19937 rng(std::random_device{}());
                                int ri = std::uniform_int_distribution<int>(0,
                                    static_cast<int>(rooms.size()) - 1)(rng);
                                m_player->setPosition(rooms[ri].centerX, rooms[ri].centerY);
                                addNotification("使用传送卷轴！传送到另一个房间。");
                            }
                            break;
                        }
                        case PotionType::PurificationPotion:
                            m_player->clearBuffs();
                            addNotification("使用净化药剂！清除了所有临时效果。");
                            break;
                    }
                    inv.removeItem(visibleIndices[m_inventoryCursor]);
                    if (m_inventoryCursor >= totalVisible - 1 && totalVisible > 1)
                        m_inventoryCursor = totalVisible - 2;
                    break;
                }
                case ItemType::Equipment: {
                    // 装备到对应槽位
                    auto& eqItem = items[visibleIndices[m_inventoryCursor]];
                    // Extract raw pointer, remove from inventory, then equip
                    auto eq = inv.removeItem(visibleIndices[m_inventoryCursor]);
                    auto* rawEq = static_cast<Equipment*>(eq.get());
                    std::string eqName = rawEq->getName();
                    m_player->equip(std::unique_ptr<Equipment>(rawEq));
                    eq.release();  // ownership transferred
                    addNotification("装备了 " + eqName + "！");
                    if (m_inventoryCursor >= totalVisible - 1 && totalVisible > 1)
                        m_inventoryCursor = totalVisible - 2;
                    break;
                }
                case ItemType::KeyItem:
                    addNotification("钥匙在锁住的楼梯处自动使用。");
                    break;
                case ItemType::QuestItem:
                    addNotification("任务物品，交给对应 NPC 完成任务。");
                    break;
            }
            break;
        }
        case 'c': case 'C':
            runForgeUI();
            break;
        case 'i': case 'I':
        case 27:  // Esc
            setState(GameState::Exploring);
            break;
    }
}

void GameEngine::handleGameOverInput() {
    if (m_lastKey == 'r' || m_lastKey == 'R') {
        m_currentFloor = 1;
        m_floorMaps.clear();
        m_savedPlayerX.clear();
        m_savedPlayerY.clear();
        m_notifications.clear();
        *m_player = Player();
        m_dungeon = std::make_unique<DungeonMap>();
        m_dungeon->generate(m_currentFloor);
        m_player->setPosition(m_dungeon->getStartX(), m_dungeon->getStartY());
        setState(GameState::Exploring);
    } else if (m_lastKey == 27) {
        m_running = false;
    }
}

// === Forge UI: self-contained rendering+input loop ===

void GameEngine::runForgeUI() {
    auto& renderer = *m_renderer;
    auto& player = *m_player;
    auto& inv = player.getInventory();

    auto& spellSystem = *m_spellSystem;
    const auto& templates = spellSystem.getTemplates();
    auto keys = spellSystem.getTemplateKeys();

    while (true) {
        renderer.clear();
        renderer.drawBox(2, 0, 78, 29, Color::Magenta);
        renderer.drawCentered(0, "═══ 法术合成台 ═══", Color::Yellow);

        // Fragment summary with counts
        auto fragments = inv.getFragments();
        std::map<std::string, int> fragCounts;
        for (auto* f : fragments) fragCounts[f->getKeyword()]++;
        std::string fragSummary;
        for (auto& [kw, cnt] : fragCounts) {
            fragSummary += "[" + kw + "]";
            if (cnt > 1) fragSummary += "x" + std::to_string(cnt);
            fragSummary += " ";
        }
        if (fragSummary.empty()) fragSummary = "(没有碎片)";
        renderer.drawStringBounded(4, 2, 76, "碎片: " + fragSummary, Color::Cyan);

        // 计算有多少配方可以合成
        int availableCount = 0;
        for (const auto& k : keys) {
            const auto& tmpl = templates.at(k);
            bool allOwned = true;
            for (const auto& frag : tmpl.requiredFragments)
                if (!inv.hasFragment(frag)) { allOwned = false; break; }
            if (allOwned) ++availableCount;
        }

        // Recipe list header with one-click synthesis
        std::string headerStr = "[0] 一键合成(" + std::to_string(availableCount) + "个)";
        headerStr += "  |  配方书 (" + std::to_string(keys.size()) + " 个法术)";
        renderer.drawStringBounded(4, 4, 76, headerStr, availableCount > 0 ? Color::Green : Color::DarkGray);

        // Show all recipes (compact 1-line format)
        int y = 6;
        for (size_t i = 0; i < keys.size() && y < 27; ++i) {
            const auto& tmpl = templates.at(keys[i]);

            // Check fragment ownership (use inv.hasFragment for permanent fragment support)
            bool allOwned = true;
            std::string fragStatus;
            for (const auto& frag : tmpl.requiredFragments) {
                bool owned = inv.hasFragment(frag);
                if (!owned) allOwned = false;
                fragStatus += std::string(owned ? "✓" : "✗") + frag + " ";
            }

            // Compact line: [N/key] name + stats + fragment checkmarks
            std::string idxLabel;
            if (i < 9) idxLabel = std::to_string(i + 1);
            else if (i == 9) idxLabel = "a";
            else if (i == 10) idxLabel = "b";
            else idxLabel = "c";
            int dmg = tmpl.damageFunc(player.getAtk());
            int hits = tmpl.hitsFunc();
            std::string line = "[" + idxLabel + "] " + tmpl.effectName;
            if (allOwned) {
                if (dmg > 0 && hits > 0)
                    line += "  D" + std::to_string(dmg) + "/" + std::to_string(hits) + "hits/"
                          + std::to_string(tmpl.baseEnergyCost) + "EP";
                else if (dmg == 0)
                    line += "  防御/治疗/" + std::to_string(tmpl.baseEnergyCost) + "EP";
                else
                    line += "  D" + std::to_string(dmg) + "/" + std::to_string(tmpl.baseEnergyCost) + "EP";
            } else {
                int missing = 0;
                for (const auto& f : tmpl.requiredFragments)
                    if (!inv.hasFragment(f)) ++missing;
                line += "  缺" + std::to_string(missing) + "碎片";
            }
            line += "  " + fragStatus;
            Color lineColor = allOwned ? Color::White : Color::DarkGray;
            renderer.drawStringBounded(5, y, 76, line, lineColor);
            ++y;
        }

        // Advanced option + escape
        renderer.drawStringBounded(4, y + 1, 76,
            "[F] 高级: 自由代码输入", Color::Cyan);
        renderer.drawStringBounded(4, y + 2, 76,
            "[Esc] 返回", Color::DarkGray);

        renderer.present();
        int ch = renderer.waitForKey();

        if (ch == 27) {
            break;  // Esc → exit forge
        }
        else if (ch == '0') {
            // === 一键合成所有可用配方 ===
            int synthesized = 0;
            std::string batchResult;
            for (size_t i = 0; i < keys.size(); ++i) {
                const auto& tmpl = templates.at(keys[i]);
                // Check if all fragments available
                bool allOwned = true;
                for (const auto& frag : tmpl.requiredFragments) {
                    if (!inv.hasFragment(frag)) { allOwned = false; break; }
                }
                if (!allOwned) continue;

                auto result = spellSystem.synthesize(static_cast<int>(i), player);
                if (result.success) {
                    // Check if spellbook full
                    auto* sb = player.getSpellBook();
                    if (sb && sb->isFull()) {
                        batchResult += "[!] 法术书已满，停止合成。\n";
                        break;
                    }
                    sb->addSpell(result.spell);
                    ++synthesized;
                }
            }
            // Show batch result
            renderer.clear();
            renderer.drawBox(10, 8, 70, 18, Color::Green);
            if (synthesized > 0) {
                renderer.drawCentered(10, "✓ 一键合成完成！", Color::Green);
                renderer.drawCentered(12, "成功合成 " + std::to_string(synthesized) + " 个法术", Color::White);
            } else if (!batchResult.empty()) {
                renderer.drawCentered(10, batchResult, Color::Yellow);
            } else {
                renderer.drawCentered(10, "✗ 没有可合成的配方", Color::Red);
                renderer.drawCentered(12, "请收集更多碎片后再试", Color::DarkGray);
            }
            renderer.drawCentered(14, "按任意键继续...", Color::DarkGray);
            renderer.present();
            renderer.waitForKey();
            // Refresh fragment counts for next render
            continue;
        }
        else if (ch == 'f' || ch == 'F') {
            // === Advanced: free-form text input ===
            renderer.clear();
            renderer.drawBox(5, 2, 75, 28, Color::Magenta);
            renderer.drawCentered(2, "═══ 高级法术合成 ═══", Color::Yellow);

            auto advFrags = inv.getFragments();
            int advY = 4;
            renderer.drawString(8, advY, "可用碎片:", Color::Cyan);
            std::string advFragList;
            for (auto* f : advFrags) advFragList += "[" + f->getKeyword() + "] ";
            if (advFragList.empty()) advFragList = "(没有碎片)";
            renderer.drawStringBounded(6, advY + 1, 73, advFragList, Color::White);

            static const std::vector<std::string> examples = {
                "for(int i=0; i<3; i++) { Attack; }     -> For Loop Combo",
                "if(Enemy HP<30%) { Counter; }          -> If Conditional Counter",
                "const int dmg=Base; Attack*dmg;        -> const Critical",
                "try { Defend; } catch(...) { Heal; }   -> try-catch Shield",
                "delete &Enemy;                         -> delete Instant Kill"
            };

            int exY = advY + 4;
            renderer.drawString(8, exY, "── 示例 ──", Color::Yellow);
            for (size_t i = 0; i < examples.size(); ++i) {
                renderer.drawStringBounded(8, exY + 1 + static_cast<int>(i), 73,
                    examples[i], Color::DarkGray);
            }

            int inputY = exY + 1 + static_cast<int>(examples.size()) + 2;
            renderer.drawString(8, inputY, "输入代码:", Color::Yellow);
            std::string userCode;
            renderer.present();

            while (true) {
                renderer.drawString(8, inputY + 1, "> " + userCode
                    + std::string(60 - userCode.length(), ' '), Color::Green);
                renderer.present();
                int ach = renderer.waitForKey();
                if (ach == '\r' || ach == '\n') break;
                if (ach == 27) { userCode = ""; break; }
                if (ach == '\b' || ach == 127) { if (!userCode.empty()) userCode.pop_back(); }
                else if (ach >= 32 && ach < 127 && userCode.length() < 55)
                    userCode += static_cast<char>(ach);
            }

            if (!userCode.empty()) {
                auto result = m_spellSystem->synthesize(userCode, player);
                showSynthesisResult(result);
            }
        }
        else if ((ch >= '1' && ch <= '9') || ch == 'a' || ch == 'b' || ch == 'c') {
            int recipeIdx;
            if (ch >= '1' && ch <= '9') recipeIdx = ch - '1';      // recipes 1-9
            else if (ch == 'a') recipeIdx = 9;                       // recipe 10
            else if (ch == 'b') recipeIdx = 10;                      // recipe 11
            else recipeIdx = 11;                                     // recipe 12
            if (recipeIdx < 0 || recipeIdx >= static_cast<int>(keys.size())) continue;
            const auto& tmpl = templates.at(keys[recipeIdx]);

            // === Detail / Confirm Screen ===
            bool allOwned = true;
            for (const auto& frag : tmpl.requiredFragments)
                if (!(fragCounts[frag] > 0)) { allOwned = false; break; }

            while (true) {
                renderer.clear();
                renderer.drawBox(8, 2, 75, 27,
                    allOwned ? Color::Green : Color::Yellow);
                renderer.drawCentered(2, "═══ " + tmpl.effectName + " ═══", Color::Yellow);

                int dy = 5;
                // Code example (highlighted)
                renderer.drawString(10, dy++, "代码:", Color::Cyan);
                renderer.drawStringBounded(12, dy++, 73,
                    "  " + tmpl.example, Color::Green);

                // Educational explanation
                dy += 1;
                renderer.drawString(10, dy++, "原理说明 (C++ 知识):", Color::Yellow);
                // Word-wrap the explanation
                std::string expl = tmpl.explanation;
                while (!expl.empty() && dy < 17) {
                    size_t len = std::min(size_t(65), expl.length());
                    if (len < expl.length()) {
                        // Find last space within limit
                        size_t sp = expl.rfind(' ', len);
                        if (sp != std::string::npos && sp > 50) len = sp;
                    }
                    renderer.drawStringBounded(12, dy++, 73,
                        expl.substr(0, len), Color::White);
                    expl = expl.substr(len);
                    while (!expl.empty() && expl[0] == ' ') expl = expl.substr(1);
                }

                // Required fragments
                dy += 1;
                std::string fragLine = "所需碎片: ";
                for (const auto& frag : tmpl.requiredFragments) {
                    bool owned = (fragCounts[frag] > 0);
                    fragLine += std::string(owned ? "✓" : "✗") + frag + " ";
                }
                renderer.drawStringBounded(10, dy++, 73, fragLine,
                    allOwned ? Color::Green : Color::Red);

                // Preview stats
                dy += 1;
                if (allOwned) {
                    int dmg = tmpl.damageFunc(player.getAtk());
                    int hits = tmpl.hitsFunc();
                    renderer.drawStringBounded(10, dy++, 73,
                        "预览: 伤害 " + std::to_string(dmg)
                        + " | 连击 " + std::to_string(hits)
                        + " | 能量 " + std::to_string(tmpl.baseEnergyCost),
                        Color::Green);
                } else {
                    renderer.drawStringBounded(10, dy++, 73,
                        "(缺少碎片 -- 无法合成)", Color::Red);
                }

                // Controls
                renderer.drawStringBounded(10, 25, 73,
                    allOwned
                        ? "[Enter] 合成此法术   [Esc] 返回配方列表"
                        : "[Esc] 返回配方列表",
                    Color::DarkGray);

                renderer.present();
                int dch = renderer.waitForKey();

                if (dch == 27) break;  // Esc → back to list
                if (dch == '\r' || dch == '\n') {
                    if (!allOwned) continue;
                    auto result = spellSystem.synthesize(recipeIdx, player);
                    showSynthesisResult(result);
                    break;
                }
            }
        }
    }
}

void GameEngine::showSynthesisResult(const SynthesisResult& result) {
    auto& renderer = *m_renderer;
    auto& player = *m_player;
    SynthesisResult r = result;  // mutable copy for message modification

    renderer.clear();
    renderer.drawBox(10, 5, 70, 22, r.success ? Color::Green : Color::Red);
    renderer.drawCentered(7,
        r.success ? "✓ 编译成功!" : "✗ 编译失败",
        r.success ? Color::Green : Color::Red);

    if (r.success) {
        auto* sb = player.getSpellBook();
        if (sb && !sb->isFull()) {
            sb->addSpell(r.spell);
        } else if (sb) {
            r.message += "\n\n[!] 法术书已满 (最多 10 个)!";
        }
    }

    std::istringstream iss(r.message);
    std::string line;
    int msgY = 9;
    while (std::getline(iss, line) && msgY < 20)
        renderer.drawStringBounded(12, msgY++, 68, line, Color::White);
    renderer.drawCentered(21, "按任意键继续...", Color::DarkGray);
    renderer.present();
    renderer.waitForKey();
}

void GameEngine::switchFloor(int targetFloor) {
    // 0. 检查当前楼层是否已清空所有怪物（用于完美清关结局）
    bool allDead = true;
    for (const auto& e : m_dungeon->getEnemies()) {
        if (e->isAlive()) { allDead = false; break; }
    }
    if (allDead && !m_player->isFloorCleared(m_currentFloor)) {
        m_player->markFloorCleared(m_currentFloor);
        addNotification("第 " + std::to_string(m_currentFloor) + " 层已清空！");
    }

    // 1. 保存当前楼层状态
    m_savedPlayerX[m_currentFloor] = m_player->getX();
    m_savedPlayerY[m_currentFloor] = m_player->getY();
    m_floorMaps[m_currentFloor] = std::move(m_dungeon);

    // 2. 切换到目标楼层
    m_currentFloor = targetFloor;

    // 3. 检查目标楼层是否已保存
    auto it = m_floorMaps.find(targetFloor);
    if (it != m_floorMaps.end()) {
        // 恢复已保存的楼层
        m_dungeon = std::move(it->second);
        m_floorMaps.erase(it);

        // 恢复玩家位置
        int savedX = m_savedPlayerX[targetFloor];
        int savedY = m_savedPlayerY[targetFloor];
        if (m_dungeon->isWalkable(savedX, savedY)) {
            m_player->setPosition(savedX, savedY);
        } else {
            m_player->setPosition(m_dungeon->getStartX(), m_dungeon->getStartY());
        }
    } else {
        // 生成新楼层
        m_dungeon = std::make_unique<DungeonMap>();
        m_dungeon->generate(targetFloor);
        m_player->setPosition(m_dungeon->getStartX(), m_dungeon->getStartY());
    }
}

void GameEngine::setState(GameState newState) {
    m_state = newState;
}

void GameEngine::addNotification(const std::string& msg) {
    m_notifications.push_back(msg);
    if (static_cast<int>(m_notifications.size()) > MAX_NOTIFICATIONS) {
        m_notifications.erase(m_notifications.begin());
    }
}

void GameEngine::runStoryScreen(const std::string& title,
                                 const std::vector<std::string>& lines,
                                 bool waitForInput) {
    auto& renderer = *m_renderer;
    renderer.clear();
    renderer.drawBox(8, 4, 72, 26, Color::Yellow);
    renderer.drawCentered(3, title, Color::Yellow);

    int y = 7;
    for (const auto& line : lines) {
        if (y >= 24) break;
        renderer.drawCentered(y, line, Color::White);
        ++y;
    }

    if (waitForInput) {
        renderer.drawCentered(25, "[按任意键继续...]", Color::DarkGray);
        renderer.present();
        renderer.waitForKey();
    } else {
        renderer.present();
    }
}

bool GameEngine::showIntro() {
    std::vector<std::string> intro = {
        "",
        "你是一名被困在传奇 IDE 中的程序员……",
        "代码地牢。",
        "",
        "腐化的代码肆意横行。语法错误潜伏在暗影中。",
        "空指针让大意者崩溃。内存泄漏消耗着你的力量。",
        "",
        "收集散落在地牢各处的代码碎片。",
        "通过编写真正的代码表达式来合成强大的法术。",
        "穿越十层地牢，面对终极魔王：",
        "SEGFAULT",
        "",
        "只有真正的代码大师才能逃离这场噩梦。",
        "愿你的编译器仁慈。",
    };
    runStoryScreen("代码地牢", intro, true);

    // Check if user pressed Esc to quit during intro
    // _getch already consumed the key, so if they didn't quit, proceed
    return true;
}

void GameEngine::showFloorStory() {
    auto& renderer = *m_renderer;
    renderer.clear();
    renderer.drawBox(10, 6, 70, 24, Color::Yellow);

    std::string title;
    std::vector<std::string> lines;

    switch (m_currentFloor) {
        case 2:
            title = "第二层 - 空指针荒原";
            lines = {
                "",
                "你深入代码地牢。",
                "空气中弥漫着悬空指针的气息……",
                "小心脚下——空指针幽灵在这些走廊中游荡。",
                "",
                "新的敌人出现了，它们带着危险的特质。",
            };
            break;
        case 3:
            title = "第三层 - 内存沼泽";
            lines = {
                "",
                "地牢变得愈发黑暗和腐化。",
                "内存泄漏像毒液一样从每一面墙渗出。",
                "栈溢出像瀑布一样倾泻而下。",
                "",
                "当心守护这一层的 Segfault！",
            };
            break;
        case 4:
            title = "第四层 - 异常深渊";
            lines = {
                "",
                "你正在接近代码地牢的核心。",
                "魔王的灵气愈发强烈……",
                "禁忌法术的碎片在黑暗中闪烁。",
                "",
                "做好准备。最终的战斗即将来临。",
            };
            break;
        case 5:
            title = "第五层 - 深渊边缘";
            lines = {
                "",
                "墙壁上布满了古老的代码铭文。",
                "空气变得稀薄，每走一步都感到沉重的压力。",
                "",
                "Segfault 的气息越来越近了……",
                "",
                "前方的黑暗更加深邃。",
                "这只是更深噩梦的开始。",
            };
            break;
        case 6:
            title = "第六层 - Segfault 的领域";
            lines = {
                "",
                "你踏入了 Segfault 的领地。",
                "魔王的身影在远处若隐若现……",
                "",
                "击败这个守护者，",
                "才能进入更深层的核心。",
                "",
                "拔剑吧，程序员！",
            };
            break;
        case 7:
            title = "第七层 - 代码深渊";
            lines = {
                "",
                "地牢开始扭曲变形。",
                "现实和代码在这里融合……",
                "",
                "Segfault 不止一个。",
                "它们似乎是某种更强大存在的分身。",
                "",
                "小心。真正的恐怖还在深处。",
            };
            break;
        case 8:
            title = "第八层 - 异常根源";
            lines = {
                "",
                "你接近了代码地牢的核心。",
                "腐败的源头在这里，伸手可及。",
                "",
                "整层楼脉动着邪恶的能量。",
                "每一个 bug 都变得更加强大。",
                "",
                "能走到这里的程序员，屈指可数。",
            };
            break;
        case 9:
            title = "第九层 - 最终 Boss";
            lines = {
                "",
                "你来到了终极挑战面前。",
                "最强的 Segfault 守卫着通往核心的道路。",
                "",
                "你的每一个法术、每一件装备、",
                "每一块碎片都汇聚成你的力量。",
                "",
                "这是最后的战斗。全力以赴！",
            };
            break;
        case 10:
            title = "第十层 - 核心";
            lines = {
                "",
                "你到达了代码地牢的核心。",
                "这里是一切开始的地方——",
                "也是这一切必须终结的地方。",
                "",
                "清除最后的腐化，",
                "为这片代码之地带来和平。",
                "",
                "你是唯一的希望。",
            };
            break;
        default:
            title = "第 " + std::to_string(m_currentFloor) + " 层";
            lines = { "", "你到达了地牢的新一层。" };
            break;
    }

    renderer.drawCentered(5, title, Color::Yellow);
    int y = 8;
    for (const auto& line : lines) {
        renderer.drawCentered(y, line, Color::White);
        ++y;
    }
    renderer.drawCentered(23, "[按任意键继续...]", Color::DarkGray);
    renderer.present();
    renderer.waitForKey();
}

void GameEngine::showNPCDialogue(NPC& npc) {
    auto& renderer = *m_renderer;
    auto& player = *m_player;

    // Count quest items player has (for quest completion check)
    auto countQuestItems = [&](int questId) -> int {
        int count = 0;
        for (const auto& item : player.getInventory().getItems()) {
            if (item->getType() == ItemType::QuestItem) {
                auto* qi = static_cast<const QuestItem*>(item.get());
                if (qi->getQuestId() == questId) ++count;
            }
        }
        return count;
    };

    // Count keys player has (for Sage quest on floor 1)
    auto countKeys = [&]() -> int {
        int count = 0;
        for (const auto& item : player.getInventory().getItems()) {
            if (item->getType() == ItemType::KeyItem) ++count;
        }
        return count;
    };

    // Determine which dialogue to show and whether to give reward
    std::vector<std::string> lines;
    bool giveReward = false;

    if (npc.questStage == 0) {
        // First meeting
        lines = npc.dialogueGreeting;
        npc.questStage = 1;  // Accept quest
    }
    else if (npc.questStage == 1) {
        // Quest in progress — check completion
        bool questDone = false;
        switch (npc.questId) {
            case 1:  // Sage: defeated 3 enemies (has 3+ keys)
                questDone = (countKeys() >= 3);
                break;
            case 2:  // Scholar: has Null Pointer Report
                questDone = (countQuestItems(2) >= 1);
                break;
            case 3:  // Keeper: has Memory Leak Detector
                questDone = (countQuestItems(3) >= 1);
                break;
            case 4:  // Ghost: has 3 Dev Journal Pages
                questDone = (countQuestItems(4) >= 3);
                break;
            default:
                questDone = true;  // No quest NPC — just show greeting again
                break;
        }

        if (questDone) {
            lines = npc.dialogueComplete;
            giveReward = true;
            npc.questStage = 2;
            player.addQuestCompleted();

            // Remove quest items used for turn-in
            if (npc.questId >= 2 && npc.questId <= 4) {
                auto& items = player.getInventory().getItems();
                int needed = (npc.questId == 4) ? 3 : 1;
                int removed = 0;
                for (size_t i = 0; i < items.size() && removed < needed; ) {
                    if (items[i]->getType() == ItemType::QuestItem) {
                        auto* qi = static_cast<QuestItem*>(items[i].get());
                        if (qi->getQuestId() == npc.questId) {
                            player.getInventory().removeItem(static_cast<int>(i));
                            ++removed;
                            continue;  // Index shifted, re-check same position
                        }
                    }
                    ++i;
                }
            }
        } else {
            lines = npc.dialogueInProgress;
        }
    }
    else {
        // Quest already completed
        lines = npc.dialoguePostQuest;
        if (lines.empty()) lines = npc.dialogueComplete;
    }

    // Render dialogue box
    renderer.clear();
    renderer.drawBox(8, 4, 72, 26, Color::Cyan);
    renderer.drawCentered(3, npc.name, Color::Cyan);

    int y = 7;
    for (const auto& line : lines) {
        if (y >= 22) break;
        renderer.drawStringBounded(10, y, 70, line, Color::White);
        ++y;
    }

    // Give reward fragment
    if (giveReward && !npc.rewardFragmentKeyword.empty() && !npc.rewardGiven) {
        y += 1;
        renderer.drawStringBounded(10, y, 70,
            "获得碎片: [" + npc.rewardFragmentKeyword + "]",
            Color::Green);

        FragmentCategory cat = CodeFragment::categoryFromKeyword(npc.rewardFragmentKeyword);
        std::string kw = npc.rewardFragmentKeyword;
        Rarity rarity = (kw == "void*") ? Rarity::Legendary : Rarity::Uncommon;
        for (int i = 0; i < npc.rewardCount; ++i) {
            player.getInventory().addItem(
                std::make_unique<CodeFragment>(kw, cat, rarity, npc.name + " 赠予"));
        }
        npc.rewardGiven = true;
    }

    renderer.drawStringBounded(10, 25, 70,
        "[按任意键继续...]", Color::DarkGray);
    renderer.present();
    renderer.waitForKey();
}

EndingType GameEngine::determineEnding() {
    int quests = m_player->getQuestsCompleted();
    int pages = m_player->getGhostPages();
    int spells = m_player->getSpellsCast();
    int hiddenRooms = m_player->getHiddenRoomsFound();
    int floorsCleared = m_player->getFloorsCleared();

    if (m_player->getFleeCount() >= 5)
        return EndingType::InfiniteLoop;   // 逃跑>=5
    if (floorsCleared >= 9)
        return EndingType::Completionist;   // 清空所有楼层（1-9层全部清空）
    if (quests == 4 && pages >= 3 && hiddenRooms >= 1)
        return EndingType::TrueEnding;      // 4任务+3残页+>=1隐藏
    if (hiddenRooms >= 3)
        return EndingType::HiddenEnding;    // >=3隐藏房间
    if (quests >= 2)
        return EndingType::Good;            // >=2任务
    if (spells == 0 && quests == 0)
        return EndingType::StackOverflow;   // 0法术+0任务
    return EndingType::Normal;
}

void GameEngine::shutdown() {
    EventBus::instance().clear();
    m_combat.reset();
    m_dungeon.reset();
    m_player.reset();
    m_renderer.reset();
}
