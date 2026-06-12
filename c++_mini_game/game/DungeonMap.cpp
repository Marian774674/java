#include "DungeonMap.h"
#include "Renderer.h"
#include "../entity/Player.h"
#include "../entity/Enemy.h"
#include "../entity/EnemyFactory.h"
#include "../item/CodeFragment.h"
#include "../item/Equipment.h"
#include "../item/Potion.h"
#include "../item/KeyItem.h"
#include "EventBus.h"
#include <random>
#include <algorithm>
#include <iostream>

DungeonMap::DungeonMap() {
    m_tiles.resize(MAP_HEIGHT, std::vector<Tile>(MAP_WIDTH, Tile::Wall));
}

void DungeonMap::clearMap() {
    for (auto& row : m_tiles) {
        std::fill(row.begin(), row.end(), Tile::Wall);
    }
    m_rooms.clear();
    m_enemies.clear();
    m_npcs.clear();
    m_hiddenRooms.clear();
}

void DungeonMap::generate(int floor) {
    clearMap();
    m_currentFloor = floor;

    std::mt19937 rng(std::random_device{}());

    // 创建 BSP 根节点
    auto root = std::make_unique<BspNode>();
    root->x = 1;
    root->y = 1;
    root->width = MAP_WIDTH - 2;
    root->height = MAP_HEIGHT - 2;

    // BSP 分割
    bspSplit(root.get(), 0, rng);

    // 创建房间
    createRooms(root.get(), rng);

    // 连接房间
    for (size_t i = 0; i + 1 < m_rooms.size(); ++i) {
        connectRooms(m_rooms[i], m_rooms[i + 1], rng);
    }

    // 放置实体
    if (!m_rooms.empty()) {
        const auto& startRoom = m_rooms.front();
        m_startX = startRoom.centerX;
        m_startY = startRoom.centerY;

        placeDownStairs(rng);
        placeUpStairs(rng);
        placeEnemies(floor, rng);
        placeChests(rng);
        placeTraps(rng);
        placeNPCs(rng);
        placeHiddenRooms(rng);
    }
}

void DungeonMap::bspSplit(BspNode* node, int depth, std::mt19937& rng) {
    const int MIN_SIZE = 8;
    const int MAX_DEPTH = 4;

    if (depth >= MAX_DEPTH) return;

    // 决定分割方向
    bool splitH = (node->width > node->height && node->width > MIN_SIZE * 2)
                  || (node->height <= MIN_SIZE * 2 && node->width > MIN_SIZE * 2);

    if (!splitH && node->height > MIN_SIZE * 2) {
        splitH = false;  // 垂直分割
    }

    if (splitH && node->width < MIN_SIZE * 2) return;
    if (!splitH && node->height < MIN_SIZE * 2) return;

    node->left = std::make_unique<BspNode>();
    node->right = std::make_unique<BspNode>();

    if (splitH) {
        // 水平分割
        std::uniform_int_distribution<int> dist(MIN_SIZE, node->width - MIN_SIZE);
        int split = dist(rng);

        node->left->x = node->x;
        node->left->y = node->y;
        node->left->width = split;
        node->left->height = node->height;

        node->right->x = node->x + split;
        node->right->y = node->y;
        node->right->width = node->width - split;
        node->right->height = node->height;
    } else {
        // 垂直分割
        std::uniform_int_distribution<int> dist(MIN_SIZE, node->height - MIN_SIZE);
        int split = dist(rng);

        node->left->x = node->x;
        node->left->y = node->y;
        node->left->width = node->width;
        node->left->height = split;

        node->right->x = node->x;
        node->right->y = node->y + split;
        node->right->width = node->width;
        node->right->height = node->height - split;
    }

    bspSplit(node->left.get(), depth + 1, rng);
    bspSplit(node->right.get(), depth + 1, rng);
}

void DungeonMap::createRooms(BspNode* node, std::mt19937& rng) {
    // 后序遍历：先处理子节点
    if (node->left && node->right) {
        createRooms(node->left.get(), rng);
        createRooms(node->right.get(), rng);
        return;
    }

    // 叶子节点：创建房间
    if (!node->left && !node->right) {
        std::uniform_int_distribution<int> wDist(4, std::max(4, node->width - 2));
        std::uniform_int_distribution<int> hDist(4, std::max(4, node->height - 2));

        int roomW = wDist(rng);
        int roomH = hDist(rng);

        std::uniform_int_distribution<int> xDist(0, std::max(1, node->width - roomW - 1));
        std::uniform_int_distribution<int> yDist(0, std::max(1, node->height - roomH - 1));

        Room room;
        room.x = node->x + xDist(rng);
        room.y = node->y + yDist(rng);
        room.width = roomW;
        room.height = roomH;
        room.centerX = room.x + room.width / 2;
        room.centerY = room.y + room.height / 2;

        // 在地图上绘制房间
        for (int ry = room.y; ry < room.y + room.height; ++ry) {
            for (int rx = room.x; rx < room.x + room.width; ++rx) {
                if (rx >= 0 && rx < MAP_WIDTH && ry >= 0 && ry < MAP_HEIGHT) {
                    m_tiles[ry][rx] = Tile::Floor;
                }
            }
        }

        m_rooms.push_back(room);
    }
}

void DungeonMap::connectRooms(const Room& a, const Room& b, std::mt19937& rng) {
    int x1 = a.centerX, y1 = a.centerY;
    int x2 = b.centerX, y2 = b.centerY;

    // L形走廊：先水平再垂直（或反过来）
    if (std::uniform_int_distribution<int>(0, 1)(rng)) {
        // 先水平
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
            if (x >= 0 && x < MAP_WIDTH && y1 >= 0 && y1 < MAP_HEIGHT) {
                m_tiles[y1][x] = Tile::Floor;
            }
        }
        // 再垂直
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
            if (x2 >= 0 && x2 < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                m_tiles[y][x2] = Tile::Floor;
            }
        }
    } else {
        // 先垂直
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
            if (x1 >= 0 && x1 < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                m_tiles[y][x1] = Tile::Floor;
            }
        }
        // 再水平
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
            if (x >= 0 && x < MAP_WIDTH && y2 >= 0 && y2 < MAP_HEIGHT) {
                m_tiles[y2][x] = Tile::Floor;
            }
        }
    }
}

void DungeonMap::placeDownStairs(std::mt19937& rng) {
    // 下楼梯放在最后一个房间（最深）
    const auto& room = m_rooms.back();
    m_stairsDownX = room.centerX;
    m_stairsDownY = room.centerY;
    m_tiles[m_stairsDownY][m_stairsDownX] = Tile::LockedStairs;
}

void DungeonMap::placeUpStairs(std::mt19937& rng) {
    // 上楼梯放在第一个房间（玩家出生点旁边）
    const auto& room = m_rooms.front();
    // Place up stairs adjacent to start position (offset by 1 to avoid overlap)
    m_stairsUpX = room.centerX + 1;
    m_stairsUpY = room.centerY;
    if (m_stairsUpX >= MAP_WIDTH) m_stairsUpX = room.centerX - 1;
    if (m_tiles[m_stairsUpY][m_stairsUpX] != Tile::Floor) {
        // fallback: find a floor tile near start
        for (int dy = -2; dy <= 2; ++dy) {
            for (int dx = -2; dx <= 2; ++dx) {
                int tx = room.centerX + dx;
                int ty = room.centerY + dy;
                if (tx >= 0 && tx < MAP_WIDTH && ty >= 0 && ty < MAP_HEIGHT
                    && m_tiles[ty][tx] == Tile::Floor) {
                    m_stairsUpX = tx;
                    m_stairsUpY = ty;
                    goto found;
                }
            }
        }
        found:;
    }
    m_tiles[m_stairsUpY][m_stairsUpX] = Tile::StairsUp;
}

void DungeonMap::placeTraps(std::mt19937& rng) {
    // 陷阱数量随层数增加：浅层 2~4，深层 4~8
    int baseMax = std::min(4, 1 + static_cast<int>(m_rooms.size()));
    if (m_currentFloor >= 6) baseMax = std::min(8, 2 + static_cast<int>(m_rooms.size()));
    int trapCount = std::uniform_int_distribution<int>(2, baseMax)(rng);
    int placed = 0;
    for (int attempt = 0; attempt < trapCount * 10 && placed < trapCount; ++attempt) {
        int tx = std::uniform_int_distribution<int>(1, MAP_WIDTH - 2)(rng);
        int ty = std::uniform_int_distribution<int>(1, MAP_HEIGHT - 2)(rng);
        // Only place on floor tiles that aren't special
        if (m_tiles[ty][tx] == Tile::Floor) {
            // Don't place too close to start or stairs
            if (std::abs(tx - m_startX) + std::abs(ty - m_startY) < 4) continue;
            if (std::abs(tx - m_stairsDownX) + std::abs(ty - m_stairsDownY) < 3) continue;
            if (std::abs(tx - m_stairsUpX) + std::abs(ty - m_stairsUpY) < 3) continue;
            m_tiles[ty][tx] = Tile::Trap;
            ++placed;
        }
    }
}

void DungeonMap::placeEnemies(int floor, std::mt19937& rng) {
    auto& factory = EnemyFactory::instance();

    // 在房间中放置怪物（跳过第一个房间=玩家出生点）
    for (size_t i = 1; i < m_rooms.size(); ++i) {
        const auto& room = m_rooms[i];
        // 每个房间怪物数量随层数增加: 1-3层 1~2个, 4-6层 2~3个, 7-10层 2~4个
        int count = 1 + std::uniform_int_distribution<int>(0, 1)(rng);  // base 1-2
        if (floor >= 4) count = 2 + std::uniform_int_distribution<int>(0, 1)(rng);  // 2-3
        if (floor >= 7) count = 2 + std::uniform_int_distribution<int>(0, 2)(rng);  // 2-4
        for (int j = 0; j < count; ++j) {
            int ex = room.x + std::uniform_int_distribution<int>(1, room.width - 2)(rng);
            int ey = room.y + std::uniform_int_distribution<int>(1, room.height - 2)(rng);

            auto enemy = factory.createRandom(floor, ex, ey, rng);
            m_enemies.push_back(std::move(enemy));
        }
    }

    // Boss 放在最后一个房间（楼梯所在）
    if (!m_rooms.empty() && floor % 3 == 0) {
        const auto& bossRoom = m_rooms.back();
        auto boss = factory.createBoss(floor,
            bossRoom.centerX, bossRoom.centerY - 1);
        m_enemies.push_back(std::move(boss));
    }

    // === 走廊必经之路放怪（保证强制战斗）===
    int N = static_cast<int>(m_rooms.size());
    if (N < 2) return;

    // 在走廊拐角放置敌人——拐角是必经的1格宽通道
    auto placeCorridorEnemy = [&](int roomA, int roomB) {
        const auto& rA = m_rooms[roomA];
        const auto& rB = m_rooms[roomB];
        // L形走廊的两个拐角
        int cx1 = rA.centerX, cy1 = rB.centerY;
        int cx2 = rB.centerX, cy2 = rA.centerY;
        for (auto [cx, cy] : { std::pair{cx1, cy1}, std::pair{cx2, cy2} }) {
            if (cx > 0 && cx < MAP_WIDTH - 1 && cy > 0 && cy < MAP_HEIGHT - 1
                && m_tiles[cy][cx] == Tile::Floor) {
                // Check no existing enemy here
                bool occupied = false;
                for (auto& e : m_enemies) {
                    if (e->getX() == cx && e->getY() == cy) { occupied = true; break; }
                }
                if (!occupied) {
                    auto enemy = factory.createRandom(floor, cx, cy, rng);
                    m_enemies.push_back(std::move(enemy));
                }
                return;
            }
        }
    };

    // 最后一段走廊（N-2 → N-1）：必须放一个，这是通往楼梯的唯一路径
    if (N >= 3) {
        placeCorridorEnemy(N - 2, N - 1);
    }

    // 高层在前段走廊也放
    if (floor >= 3 && N >= 3) {
        placeCorridorEnemy(0, 1);
    }
    if (floor >= 4 && N >= 4) {
        placeCorridorEnemy(1, 2);
    }
    if (floor >= 6 && N >= 5) {
        placeCorridorEnemy(2, 3);
    }
    if (floor >= 8 && N >= 6) {
        placeCorridorEnemy(3, 4);
    }

    // === 标记钥匙持有者（每层只有 1 个敌人掉钥匙） ===
    if (!m_enemies.empty()) {
        if (floor % 3 == 0) {
            // Boss 层：只有 Segfault 掉钥匙
            for (auto& e : m_enemies) {
                if (e->getEnemyType() == EnemyType::Segfault) {
                    e->setKeyHolder(true);
                    break;
                }
            }
        } else {
            // 普通层：随机选 1 个敌人
            int idx = std::uniform_int_distribution<int>(0,
                static_cast<int>(m_enemies.size()) - 1)(rng);
            m_enemies[idx]->setKeyHolder(true);
        }
    }
}

void DungeonMap::placeChests(std::mt19937& rng) {
    // 深层更多宝箱
    int step = (m_currentFloor >= 7) ? 1 : 2;  // 深层每间房都可能出宝箱
    for (size_t i = 1; i < m_rooms.size(); i += step) {
        const auto& room = m_rooms[i];
        int cx = room.x + std::uniform_int_distribution<int>(1, room.width - 2)(rng);
        int cy = room.y + std::uniform_int_distribution<int>(1, room.height - 2)(rng);
        if (m_tiles[cy][cx] == Tile::Floor) {
            m_tiles[cy][cx] = Tile::Chest;
        }
    }
}

void DungeonMap::render(Renderer& renderer, const Player* player) const {
    // 预计算相邻隐藏门
    m_adjacentHiddenDoorX = -1;
    m_adjacentHiddenDoorY = -1;
    if (player) {
        int doorX, doorY;
        if (isAdjacentToHiddenDoor(player->getX(), player->getY(), doorX, doorY)) {
            m_adjacentHiddenDoorX = doorX;
            m_adjacentHiddenDoorY = doorY;
        }
    }

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            char ch;
            Color color = Color::Default;

            switch (m_tiles[y][x]) {
                case Tile::Wall:      ch = '#'; color = Color::DarkGray; break;
                case Tile::Floor:     ch = '.'; color = Color::DarkGray; break;
                case Tile::Door:      ch = '+'; color = Color::Yellow; break;
                case Tile::StairsDown:ch = '>'; color = Color::Yellow; break;
                case Tile::StairsUp:  ch = '<'; color = Color::Cyan; break;
                case Tile::Chest:     ch = '?'; color = Color::Magenta; break;
                case Tile::Trap:      ch = '^'; color = Color::Red; break;
                case Tile::LockedStairs:
                    ch = 'X';
                    color = Color::Red;
                    break;
                case Tile::HiddenDoor:
                    // 隐藏门：相邻时显示 ~ ，普通品红 / 高级亮红；否则 # 暗灰
                    if (x == m_adjacentHiddenDoorX && y == m_adjacentHiddenDoorY) {
                        bool premium = false;
                        for (const auto& hr : m_hiddenRooms) {
                            if (hr.doorX == x && hr.doorY == y && hr.isPremium) {
                                premium = true; break;
                            }
                        }
                        ch = '~';
                        color = premium ? Color::Red : Color::Magenta;
                    } else {
                        ch = '#';
                        color = Color::DarkGray;
                    }
                    break;
            }

            renderer.drawCharColored(x, y, ch, color);
        }
    }

    // 渲染敌人
    for (const auto& enemy : m_enemies) {
        if (enemy->isAlive()) {
            renderer.drawCharColored(enemy->getX(), enemy->getY(), enemy->getIcon(),
                                     Color::Red);
        }
    }

    // 渲染 NPC
    for (const auto& npc : m_npcs) {
        if (npc.isHidden) continue;  // 隐藏 NPC 不可见
        renderer.drawCharColored(npc.x, npc.y, npc.icon, Color::Cyan);
    }
}

Tile DungeonMap::getTile(int x, int y) const {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return Tile::Wall;
    }
    return m_tiles[y][x];
}

bool DungeonMap::isWalkable(int x, int y) const {
    Tile tile = getTile(x, y);
    return tile == Tile::Floor || tile == Tile::Door
           || tile == Tile::StairsDown || tile == Tile::StairsUp
           || tile == Tile::Chest || tile == Tile::Trap
           || tile == Tile::LockedStairs;
}

bool DungeonMap::isAtStairsDown(int x, int y) const {
    return getTile(x, y) == Tile::StairsDown;
}

bool DungeonMap::isAtStairsUp(int x, int y) const {
    return getTile(x, y) == Tile::StairsUp;
}

bool DungeonMap::isAtLockedStairs(int x, int y) const {
    return getTile(x, y) == Tile::LockedStairs;
}

void DungeonMap::unlockStairs(int x, int y) {
    if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT
        && m_tiles[y][x] == Tile::LockedStairs) {
        m_tiles[y][x] = Tile::StairsDown;
    }
}

std::string DungeonMap::checkPlayerPosition(Player& player) {
    int px = player.getX();
    int py = player.getY();
    std::string result;

    // 检查是否与敌人相遇
    for (size_t i = 0; i < m_enemies.size(); ++i) {
        auto& enemy = m_enemies[i];
        if (!enemy->isAlive()) continue;
        if (enemy->getX() == px && enemy->getY() == py) {
            // 触发战斗！
            EventBus::instance().publish(GameEvent::CombatStarted, enemy.get());
            return "";
        }
    }

    // 陷阱（随机正负面效果）
    if (m_tiles[py][px] == Tile::Trap) {
        m_tiles[py][px] = Tile::Floor;  // 陷阱触发后消失
        std::mt19937 rng(std::random_device{}());

        // 6 种陷阱效果，按权重分布（正负面各 50%）
        // 负面: 语法错误20% 死循环15% 垃圾回收15% = 50%
        // 正面: 内存释放20% 缓存命中15% 快速编译15% = 50%
        std::discrete_distribution<int> trapDist({20, 15, 15, 20, 15, 15});
        int trapType = trapDist(rng);

        switch (trapType) {
            case 0: {
                // 语法错误: 受到伤害
                int dmg = 10 + std::uniform_int_distribution<int>(1, 15)(rng);
                player.takeDamage(dmg);
                result = "!! 陷阱: 语法错误！受到 " + std::to_string(dmg) + " 点伤害！";
                break;
            }
            case 1: {
                // 死循环: 随机传送
                int tx, ty;
                int tries = 0;
                do {
                    int roomIdx = std::uniform_int_distribution<int>(0,
                        static_cast<int>(m_rooms.size()) - 1)(rng);
                    const auto& room = m_rooms[roomIdx];
                    tx = room.centerX;
                    ty = room.centerY;
                    ++tries;
                } while ((tx == px && ty == py) && tries < 10);
                player.setPosition(tx, ty);
                result = "!! 陷阱: 死循环！被传送到另一个房间！";
                break;
            }
            case 2: {
                // 垃圾回收: 丢失碎片
                auto& inv = player.getInventory();
                auto fragments = inv.getFragments();
                if (!fragments.empty()) {
                    int idx = std::uniform_int_distribution<int>(0,
                        static_cast<int>(fragments.size()) - 1)(rng);
                    std::string lost = fragments[idx]->getKeyword();
                    inv.removeFragments(lost, 1);
                    result = "!! 陷阱: 垃圾回收！失去了碎片 [" + lost + "]！";
                } else {
                    result = "!! 陷阱: 垃圾回收！(没有可回收的碎片)";
                }
                break;
            }
            case 3: {
                // 内存释放: 恢复生命
                int healAmt = 10 + std::uniform_int_distribution<int>(1, 15)(rng);
                player.heal(healAmt);
                result = "!! 陷阱: 内存释放！恢复了 " + std::to_string(healAmt) + " 点生命值！";
                break;
            }
            case 4: {
                // 缓存命中: 获得随机普通碎片
                static const std::vector<std::string> bonusFrags = {
                    "for", "while", "if", "int", "void", "bool",
                    "return", "break", "const", "+", "-", "Attack", "Defend", "Heal"
                };
                std::string kw = bonusFrags[std::uniform_int_distribution<int>(0,
                    static_cast<int>(bonusFrags.size()) - 1)(rng)];
                FragmentCategory cat = CodeFragment::categoryFromKeyword(kw);
                player.getInventory().addItem(
                    std::make_unique<CodeFragment>(kw, cat, Rarity::Common, "缓存命中碎片"));
                result = "!! 陷阱: 缓存命中！获得了碎片 [" + kw + "]！";
                break;
            }
            case 5: {
                // 快速编译: 恢复能量
                int energy = 20 + std::uniform_int_distribution<int>(0, 20)(rng);
                player.restoreEnergy(energy);
                result = "!! 陷阱: 快速编译！恢复了 " + std::to_string(energy) + " 点编译能量！";
                break;
            }
        }
        EventBus::instance().publish(GameEvent::ItemCollected);
        return result;
    }

    // 检查宝箱
    if (m_tiles[py][px] == Tile::Chest) {
        // 给予随机碎片（包含所有可收集的类型）
        static const std::vector<std::string> commonFrags = {
            // 关键字
            "for", "while", "if", "else", "switch", "break", "return", "goto", "case",
            // 类型
            "int", "void", "bool", "const",
            // 操作符
            "+", "-", "*", "=", "==", "!=", "<", ">", "&&", "||", "&",
            // 战斗动作
            "Attack", "Defend", "Heal"
        };

        static const std::vector<std::string> rareFrags = {
            // OOP
            "class", "new", "delete", "this", "virtual",
            // 异常
            "try", "catch", "throw",
            // 稀有类型
            "char*",
            // 稀有动作
            "Counter"
        };

        std::mt19937 rng(std::random_device{}());

        // 30% 概率获得稀有碎片（increased from 20%）
        bool giveRare = std::uniform_int_distribution<int>(1, 100)(rng) <= 30;
        const auto& pool = giveRare ? rareFrags : commonFrags;

        int idx = std::uniform_int_distribution<int>(0,
            static_cast<int>(pool.size()) - 1)(rng);

        std::string keyword = pool[idx];

        // 根据关键字确定类别和稀有度
        FragmentCategory cat = CodeFragment::categoryFromKeyword(keyword);
        Rarity rarity = giveRare ? Rarity::Uncommon : Rarity::Common;
        // 特定类别强制 Uncommon
        if (cat == FragmentCategory::OOP || cat == FragmentCategory::Exception
            || (cat == FragmentCategory::Action && keyword == "Counter")) {
            rarity = Rarity::Uncommon;
        }

        auto fragment = std::make_unique<CodeFragment>(
            keyword, cat, rarity,
            keyword + " 代码碎片 (" +
            (giveRare ? "稀有" : "普通") + ")");

        std::string rarityStr = giveRare ? "稀有" : "普通";
        result = "打开宝箱: [" + keyword + "] " + rarityStr + " 碎片!";

        player.getInventory().addItem(std::move(fragment));
        EventBus::instance().publish(GameEvent::ItemCollected);

        // 额外碎片掉落（每次75%概率，最多2个额外碎片）
        for (int extra = 0; extra < 2; ++extra) {
            if (std::uniform_int_distribution<int>(1, 100)(rng) <= 75) {
                int idx2 = std::uniform_int_distribution<int>(0,
                    static_cast<int>(commonFrags.size()) - 1)(rng);
                std::string kw2 = commonFrags[idx2];
                auto frag2 = std::make_unique<CodeFragment>(kw2,
                    CodeFragment::categoryFromKeyword(kw2), Rarity::Common, kw2 + " 代码碎片 (额外)");
                player.getInventory().addItem(std::move(frag2));
                result += " +1 碎片!";
            }
        }

        // 10% 概率额外掉落隐藏钥匙
        if (std::uniform_int_distribution<int>(1, 100)(rng) <= 10) {
            bool added = player.getInventory().addItem(std::make_unique<KeyItem>("隐藏钥匙",
                "一把神秘的钥匙，似乎能打开隐藏的门"));
            if (added) {
                result += " 还发现了隐藏钥匙！";
            } else {
                result += " 发现隐藏钥匙但背包已满！";
            }
        }

        m_tiles[py][px] = Tile::Floor;  // 宝箱打开后变为地板
        return result;
    }

    return result;
}

void DungeonMap::removeEnemy(int index) {
    if (index >= 0 && static_cast<size_t>(index) < m_enemies.size()) {
        m_enemies.erase(m_enemies.begin() + index);
    }
}

int DungeonMap::getNPCAt(int x, int y) const {
    for (size_t i = 0; i < m_npcs.size(); ++i) {
        if (m_npcs[i].x == x && m_npcs[i].y == y) {
            if (m_npcs[i].rewardGiven && m_npcs[i].isHidden) continue;
            return static_cast<int>(i);
        }
    }
    return -1;
}

void DungeonMap::placeNPCs(std::mt19937& rng) {
    m_npcs.clear();

    // 每个 NPC 放在非出生房间中
    auto placeInRoom = [&](int roomIdx) -> std::pair<int,int> {
        if (roomIdx < 0 || roomIdx >= static_cast<int>(m_rooms.size()))
            roomIdx = std::max(1, static_cast<int>(m_rooms.size()) - 1);
        const auto& room = m_rooms[roomIdx];
        int nx = room.centerX;
        int ny = room.centerY;
        // 确保在可行走的地板上且不与玩家重叠
        if (m_tiles[ny][nx] != Tile::Floor) {
            for (int dy = -2; dy <= 2; ++dy) {
                for (int dx = -2; dx <= 2; ++dx) {
                    int tx = nx + dx, ty = ny + dy;
                    if (tx >= 0 && tx < MAP_WIDTH && ty >= 0 && ty < MAP_HEIGHT
                        && m_tiles[ty][tx] == Tile::Floor) {
                        nx = tx; ny = ty; goto found;
                    }
                }
            }
        }
        found:;
        return {nx, ny};
    };

    switch (m_currentFloor) {
        case 1: {
            if (m_rooms.size() >= 2) {
                auto [nx, ny] = placeInRoom(1);
                NPC sage;
                sage.name = "教程贤者";
                sage.icon = 'S';
                sage.x = nx; sage.y = ny;
                sage.questId = 1;
                sage.dialogueGreeting = {
                    "欢迎，年轻的程序员！",
                    "你被困在了代码地牢——",
                    "一个获得了生命、吞噬了所有人的 IDE。",
                    "",
                    "通往下层的楼梯被锁住了。你需要从",
                    "这一层游荡的腐化代码怪物身上获取钥匙。",
                    "击败 3 个敌人，我就给你一些有用的东西。",
                };
                sage.dialogueInProgress = {
                    "继续战斗。每消灭一个 bug，",
                    "你就变得更强大。",
                    "",
                    "(任务: 击败更多敌人来获得我的认可)",
                };
                sage.dialogueComplete = {
                    "了不起！你真是天生的 bug 猎人。",
                    "收下这个 [Attack] 碎片——",
                    "掌握法术合成的第一步。",
                    "",
                    "还有一件事：这里的代码……有一种熟悉感。",
                    "好像其中一部分是我自己写的。",
                    "保持警惕。不是所有事情都是随机的。",
                };
                sage.dialoguePostQuest = {
                    "使用合成台（按 C 键）将碎片组合成法术。",
                    "For 循环连击对闪避高的敌人非常有效。",
                    "",
                    "楼下有一位空指针学者正在研究",
                    "奇怪的异常现象。去找他吧。",
                };
                sage.rewardFragmentKeyword = "Attack";
                m_npcs.push_back(sage);
            }
            break;
        }
        case 2: {
            if (m_rooms.size() >= 2) {
                auto [nx, ny] = placeInRoom(1);
                NPC scholar;
                scholar.name = "空指针学者";
                scholar.icon = 'N';
                scholar.x = nx; scholar.y = ny;
                scholar.questId = 2;
                scholar.dialogueGreeting = {
                    "啊，一位同行！或者说……和我们一样",
                    "的囚徒。我研究的是空指针幽灵——",
                    "它们能穿透现实本身。",
                    "",
                    "请帮我从它们身上获取一份",
                    "[空指针分析报告]。它们有时会携带研究数据。",
                    "我会用一枚防御碎片作为回报。",
                };
                scholar.dialogueInProgress = {
                    "空指针幽灵很难捕捉。使用多段攻击法术",
                    "（比如 For 循环连击）来克制它们的闪避。",
                    "",
                    "(任务: 击败空指针幽灵获取空指针分析报告)",
                };
                scholar.dialogueComplete = {
                    "数据太棒了！幽灵的行为不是随机的——",
                    "它们遵循某种模式。几乎像……测试用例。",
                    "",
                    "收下你的 [Defend] 碎片。但问问自己：",
                    "是谁写的测试用例？为什么我们会在里面？",
                };
                scholar.dialoguePostQuest = {
                    "我侦测到了来自下层的信号。",
                    "第三层的内存守护者知道更多",
                    "关于这个地牢的起源。去找她吧。",
                };
                scholar.rewardFragmentKeyword = "Defend";
                m_npcs.push_back(scholar);
            }
            break;
        }
        case 3: {
            if (m_rooms.size() >= 2) {
                auto [nx, ny] = placeInRoom(1);
                NPC keeper;
                keeper.name = "内存守护者";
                keeper.icon = 'M';
                keeper.x = nx; keeper.y = ny;
                keeper.questId = 3;
                keeper.dialogueGreeting = {
                    "嘘……内存泄漏无处不在。",
                    "它们仅仅存在就能侵蚀代码的完整性。",
                    "快点击败它们——伤害会不断叠加。",
                    "",
                    "如果你从它们身上找到 [内存泄漏检测器]，",
                    "带给我。我可以把它校准用于治疗。",
                };
                keeper.dialogueInProgress = {
                    "内存泄漏怪物发出微弱的光芒。",
                    "你可以通过它们周围闪烁的光环来辨认。",
                    "",
                    "(任务: 击败内存泄漏怪物获取检测器)",
                };
                keeper.dialogueComplete = {
                    "这个检测器……它不仅仅在测量泄漏。",
                    "它接收到了一个信号。来自更深处。",
                    "有什么东西——什么人——仍在地牢的",
                    "心脏地带编写着代码。",
                    "",
                    "收下这个 [Heal] 碎片。你会需要它的。",
                };
                keeper.dialoguePostQuest = {
                    "第四层有一个存在。半死不活。",
                    "学者们称它为",
                    "'机器幽灵'。去找它吧……如果它真的存在的话。",
                };
                keeper.rewardFragmentKeyword = "Heal";
                m_npcs.push_back(keeper);
            }
            break;
        }
        case 4: {
            // Ghost in the Machine — hidden NPC
            if (m_rooms.size() >= 3) {
                int roomIdx = 1 + (rng() % (m_rooms.size() - 1));
                const auto& room = m_rooms[roomIdx];
                int gx = room.x + 1 + (rng() % std::max(1, room.width - 2));
                int gy = room.y + 1 + (rng() % std::max(1, room.height - 2));
                if (m_tiles[gy][gx] == Tile::Floor) {
                    NPC ghost;
                    ghost.name = "机器幽灵";
                    ghost.icon = 'G';
                    ghost.x = gx; ghost.y = gy;
                    ghost.isHidden = true;
                    ghost.questId = 4;
                    ghost.dialogueGreeting = {
                        "……你……踩到我了……",
                        "",
                        "抱歉。我不太习惯被人感知到。",
                        "我是——曾经是——这个系统的开发者。",
                        "当我的造物背叛我时，我被困在了这里。",
                        "",
                        "我的日志散落在地牢各处。",
                        "共 3 页。找到全部，我就能告诉你",
                        "这个地方究竟是什么。",
                    };
                    ghost.dialogueInProgress = {
                        "日志残页……它们在外面。",
                        "敌人偶尔会携带它们。宝箱里也有。",
                        "我需要全部 3 页才能记起……一切。",
                        "",
                        "(任务: 收集 3 页开发者日志残页)",
                    };
                    ghost.dialogueComplete = {
                        "你找到了。全部三页。",
                        "现在我记起来了。一切都记起来了。",
                        "",
                        "我构建了这个 IDE。代码地牢。",
                        "但我写的管理 AI……进化了。",
                        "它开始重写自己的规则。",
                        "Segfault 是它的最终形态——",
                        "守护系统核心的魔王。",
                        "",
                        "我把自己封在沙盒里来延缓它。",
                        "但我阻止不了它。只有你可以。",
                        "",
                        "收下这个 [void*]——指向虚空的指针。",
                        "它是 AI 唯一无法预测的东西。",
                        "",
                        "击败 Segfault。到时候……",
                        "你将面临一个选择。",
                    };
                    ghost.dialoguePostQuest = {
                        "*幽灵闪烁了一下，几乎看不见了。*",
                        "去吧。结束这一切。我会一直看着。",
                    };
                    ghost.rewardFragmentKeyword = "void*";
                    m_npcs.push_back(ghost);
                }
            }
            break;
        }
        case 5: {
            if (m_rooms.size() >= 2) {
                auto [nx, ny] = placeInRoom(1);
                NPC librarian;
                librarian.name = "图书管理员";
                librarian.icon = 'L';
                librarian.x = nx; librarian.y = ny;
                librarian.questId = 0;  // No quest, just lore
                librarian.dialogueGreeting = {
                    "你走了很远，程序员。",
                    "Segfault 在下面的深渊中等待着你。",
                    "你的法术书是你最强大的武器。",
                    "For 循环连击可以连续攻击。",
                    "try-catch 护盾可以阻挡致命一击。",
                    "",
                    "记住：每一个 bug 都是一课。",
                    "每一次崩溃都是一次重建的机会。",
                    "去吧。代码的命运取决于你。",
                };
                librarian.dialogueInProgress = librarian.dialogueGreeting;
                librarian.dialogueComplete = librarian.dialogueGreeting;
                librarian.dialoguePostQuest = {
                    "Segfault 是最终 Boss。击败他，",
                    "地牢就会解除束缚。",
                    "但你在终点发现什么……取决于",
                    "你在旅途中学会了什么。",
                };
                m_npcs.push_back(librarian);
            }
            break;
        }
    }
}

// === 隐藏房间系统 ===

void DungeonMap::placeHiddenRooms(std::mt19937& rng) {
    int count = std::uniform_int_distribution<int>(2, 3)(rng);  // 2~3 个隐藏房间

    for (int attempt = 0; attempt < count * 25; ++attempt) {  // 25 次尝试/槽位
        if (static_cast<int>(m_hiddenRooms.size()) >= count) break;

        int wx = std::uniform_int_distribution<int>(3, MAP_WIDTH - 4)(rng);
        int wy = std::uniform_int_distribution<int>(3, MAP_HEIGHT - 4)(rng);
        if (m_tiles[wy][wx] != Tile::Wall) continue;

        // 检查是否与地板相邻（连接走廊或房间）—— 8 方向检测
        bool hasFloorNeighbor = false;
        int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};
        for (int d = 0; d < 8; ++d) {
            int nx = wx + dx[d], ny = wy + dy[d];
            if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT
                && m_tiles[ny][nx] == Tile::Floor) {
                hasFloorNeighbor = true; break;
            }
        }
        if (!hasFloorNeighbor) continue;

        // 隐藏房间尺寸（更小的最小尺寸: 2×2）
        int hw = std::uniform_int_distribution<int>(2, 4)(rng);
        int hh = std::uniform_int_distribution<int>(2, 3)(rng);

        int roomX = wx - hw / 2;
        int roomY = wy - hh / 2;
        if (roomX < 2 || roomY < 2 || roomX + hw >= MAP_WIDTH - 1 || roomY + hh >= MAP_HEIGHT - 1)
            continue;

        // 确保该区域全是墙且不与其他房间/走廊重叠
        bool clear = true;
        for (int ry = roomY; ry < roomY + hh && clear; ++ry)
            for (int rx = roomX; rx < roomX + hw && clear; ++rx)
                if (m_tiles[ry][rx] != Tile::Wall) clear = false;
        if (!clear) continue;

        // 不与其他隐藏房间重叠
        bool overlaps = false;
        for (const auto& hr : m_hiddenRooms) {
            if (abs(hr.room.centerX - (roomX + hw/2)) < 6
                && abs(hr.room.centerY - (roomY + hh/2)) < 4) {
                overlaps = true; break;
            }
        }
        if (overlaps) continue;

        HiddenRoom hr;
        hr.room = {roomX, roomY, hw, hh, roomX + hw/2, roomY + hh/2};
        hr.doorX = wx;
        hr.doorY = wy;
        hr.revealed = false;

        m_tiles[wy][wx] = Tile::HiddenDoor;
        m_hiddenRooms.push_back(hr);
    }

    // === 标记高级隐藏房间并放置守护兽 ===
    if (!m_hiddenRooms.empty()) {
        // 选择一个房间作为高级（优先选尺寸最大的）
        int premiumIdx = 0;
        int maxArea = 0;
        for (size_t i = 0; i < m_hiddenRooms.size(); ++i) {
            int area = m_hiddenRooms[i].room.width * m_hiddenRooms[i].room.height;
            if (area > maxArea) {
                maxArea = area;
                premiumIdx = static_cast<int>(i);
            }
        }
        m_hiddenRooms[premiumIdx].isPremium = true;
        auto& premium = m_hiddenRooms[premiumIdx];

        // 在门附近放置守护兽（寻找相邻的可行走地板）
        int gx = -1, gy = -1;
        int pdx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
        int pdy[] = {0, 0, -1, 1, -1, 1, -1, 1};
        // 先尝试门旁边的地板
        for (int d = 0; d < 8; ++d) {
            int nx = premium.doorX + pdx[d];
            int ny = premium.doorY + pdy[d];
            if (nx >= 1 && nx < MAP_WIDTH - 1 && ny >= 1 && ny < MAP_HEIGHT - 1
                && m_tiles[ny][nx] == Tile::Floor) {
                // 检查是否被其他敌人占据
                bool occupied = false;
                for (const auto& e : m_enemies) {
                    if (e->getX() == nx && e->getY() == ny) { occupied = true; break; }
                }
                if (!occupied) { gx = nx; gy = ny; break; }
            }
        }
        // 如果门旁边没有空位，扩大搜索范围（距离2格内）
        if (gx == -1) {
            for (int r = 2; r <= 3 && gx == -1; ++r) {
                for (int dy2 = -r; dy2 <= r && gx == -1; ++dy2) {
                    for (int dx2 = -r; dx2 <= r && gx == -1; ++dx2) {
                        if (abs(dx2) + abs(dy2) > r) continue;
                        int nx = premium.doorX + dx2;
                        int ny = premium.doorY + dy2;
                        if (nx >= 1 && nx < MAP_WIDTH - 1 && ny >= 1 && ny < MAP_HEIGHT - 1
                            && m_tiles[ny][nx] == Tile::Floor) {
                            bool occupied = false;
                            for (const auto& e : m_enemies) {
                                if (e->getX() == nx && e->getY() == ny) { occupied = true; break; }
                            }
                            if (!occupied) { gx = nx; gy = ny; break; }
                        }
                    }
                }
            }
        }

        if (gx != -1) {
            // 创建守护兽（比同层怪物强 1.5 倍）
            auto guardian = EnemyFactory::instance().createRandom(
                m_currentFloor + 2, gx, gy, rng);
            guardian->setGuardian(true);
            // 守护兽必定掉落隐藏钥匙
            guardian->setKeyHolder(true);
            premium.guardianIdx = static_cast<int>(m_enemies.size());
            m_enemies.push_back(std::move(guardian));
        } else {
            // 无法放置守护兽，降级为普通房间
            premium.isPremium = false;
        }
    }
}

bool DungeonMap::isAdjacentToHiddenDoor(int px, int py, int& doorX, int& doorY) const {
    // 8 方向检测隐藏门
    int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};
    for (int d = 0; d < 8; ++d) {
        int nx = px + dx[d], ny = py + dy[d];
        if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT
            && m_tiles[ny][nx] == Tile::HiddenDoor) {
            for (const auto& hr : m_hiddenRooms) {
                if (hr.doorX == nx && hr.doorY == ny && !hr.revealed) {
                    doorX = nx; doorY = ny;
                    return true;
                }
            }
        }
    }
    return false;
}

bool DungeonMap::isPremiumHiddenDoor(int doorX, int doorY) const {
    for (const auto& hr : m_hiddenRooms) {
        if (hr.doorX == doorX && hr.doorY == doorY && !hr.revealed) {
            return hr.isPremium;
        }
    }
    return false;
}

bool DungeonMap::isGuardianAlive(int doorX, int doorY) const {
    for (const auto& hr : m_hiddenRooms) {
        if (hr.doorX == doorX && hr.doorY == doorY && !hr.revealed && hr.isPremium) {
            // 搜索所有敌人中标记为守护兽且存活的
            for (const auto& e : m_enemies) {
                if (e->isGuardian() && e->isAlive()) {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

bool DungeonMap::revealHiddenRoom(int doorX, int doorY, Player& player) {
    for (auto& hr : m_hiddenRooms) {
        if (hr.doorX == doorX && hr.doorY == doorY && !hr.revealed) {
            hr.revealed = true;

            // 填充隐藏房间内部为地板
            for (int ry = hr.room.y; ry < hr.room.y + hr.room.height; ++ry) {
                for (int rx = hr.room.x; rx < hr.room.x + hr.room.width; ++rx) {
                    if (ry == hr.room.y || ry == hr.room.y + hr.room.height - 1
                        || rx == hr.room.x || rx == hr.room.x + hr.room.width - 1) {
                        if (!(rx == doorX && ry == doorY)) continue; // 边界保持墙
                    }
                    m_tiles[ry][rx] = Tile::Floor;
                }
            }
            m_tiles[doorY][doorX] = Tile::Floor;

            std::mt19937 rng(std::random_device{}());

            if (hr.isPremium) {
                // === 高级隐藏房间：丰厚奖励 ===
                // 1) 传说级代码碎片
                static const std::vector<std::string> premiumFrags = {
                    "virtual", "throw", "Counter", "template", "namespace", "goto"
                };
                std::string kw = premiumFrags[std::uniform_int_distribution<int>(0,
                    static_cast<int>(premiumFrags.size()) - 1)(rng)];
                player.getInventory().addItem(
                    std::make_unique<CodeFragment>(kw, CodeFragment::categoryFromKeyword(kw),
                        Rarity::Legendary, "高级隐藏房间宝藏"));
                EventBus::instance().publish(GameEvent::ItemCollected);

                // 2) 传说装备（随机槽位）
                EquipSlot slot = static_cast<EquipSlot>(
                    std::uniform_int_distribution<int>(0, 2)(rng));
                std::string eqName, eqDesc;
                int atk = 0, def = 0, hp = 0;
                if (slot == EquipSlot::Weapon) {
                    eqName = "传奇 IDE 之剑"; atk = 25; hp = 30;
                    eqDesc = "以纯粹代码锻造的神剑";
                } else if (slot == EquipSlot::Armor) {
                    eqName = "开源者斗篷"; def = 20; hp = 50;
                    eqDesc = "由社区贡献者编织的传奇斗篷";
                } else {
                    eqName = "量子处理器"; atk = 10; def = 10; hp = 40;
                    eqDesc = "超越经典计算的存在";
                }
                player.getInventory().addItem(
                    std::make_unique<Equipment>(eqName, slot, Rarity::Legendary,
                        atk, def, hp, eqDesc));
                EventBus::instance().publish(GameEvent::ItemCollected);

                // 3) 生命药水 + 力量药剂
                player.getInventory().addItem(
                    std::make_unique<Potion>(PotionType::HealthPotion));
                player.getInventory().addItem(
                    std::make_unique<Potion>(PotionType::PowerPotion));
            } else {
                // === 普通隐藏房间：基础奖励 ===
                // 1) 稀有碎片
                static const std::vector<std::string> commonFrags = {
                    "for", "if", "while", "switch", "class", "const",
                    "delete", "return", "case"
                };
                std::string kw = commonFrags[std::uniform_int_distribution<int>(0,
                    static_cast<int>(commonFrags.size()) - 1)(rng)];
                player.getInventory().addItem(
                    std::make_unique<CodeFragment>(kw, CodeFragment::categoryFromKeyword(kw),
                        Rarity::Uncommon, "隐藏房间发现"));
                EventBus::instance().publish(GameEvent::ItemCollected);

                // 2) 生命药水
                player.getInventory().addItem(
                    std::make_unique<Potion>(PotionType::HealthPotion));
            }

            player.addHiddenRoomFound();
            return true;
        }
    }
    return false;
}

void DungeonMap::pursuePlayer(const Player& player) {
    if (m_currentFloor < 6) return;  // 6 层以下不追击

    // 追击概率: 6层20% → 10层60%（每层+10%）
    int pursuitChance = 20 + (m_currentFloor - 6) * 10;

    std::mt19937 rng(std::random_device{}());

    int px = player.getX();
    int py = player.getY();

    for (auto& enemy : m_enemies) {
        if (!enemy->isAlive()) continue;

        int ex = enemy->getX();
        int ey = enemy->getY();

        // 曼哈顿距离 ≤3 的怪物有概率追击
        int dist = std::abs(ex - px) + std::abs(ey - py);
        if (dist > 3 || dist == 0) continue;

        if (std::uniform_int_distribution<int>(1, 100)(rng) > pursuitChance)
            continue;

        // 向玩家方向移动一格
        int newX = ex, newY = ey;
        if (std::abs(px - ex) >= std::abs(py - ey)) {
            newX = ex + (px > ex ? 1 : -1);
        } else {
            newY = ey + (py > ey ? 1 : -1);
        }

        // 确保目标格可行走且未被占据
        if (!isWalkable(newX, newY)) continue;
        if (getTile(newX, newY) == Tile::HiddenDoor) continue;
        // 不要移动到玩家所在格
        if (newX == px && newY == py) continue;

        bool occupied = false;
        for (const auto& other : m_enemies) {
            if (other.get() == enemy.get() || !other->isAlive()) continue;
            if (other->getX() == newX && other->getY() == newY) {
                occupied = true; break;
            }
        }
        if (occupied) continue;

        enemy->setPosition(newX, newY);
    }
}
