#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <random>
#include "../entity/NPC.h"

class Renderer;
class Player;
class Enemy;

// 地图单元格类型
enum class Tile {
    Wall,        // # 墙
    Floor,       // . 地板
    Door,        // + 门
    StairsDown,  // > 下楼梯
    StairsUp,    // < 上楼梯
    Chest,       // ? 宝箱
    Trap,        // ^ 陷阱
    LockedStairs,  // X 锁住的楼梯（需要钥匙）
    HiddenDoor     // ~ 隐藏门（相邻时可见）
};

// 房间结构
struct Room {
    int x, y;           // 左上角
    int width, height;  // 尺寸
    int centerX, centerY; // 中心坐标
};

// BSP 树节点
struct BspNode {
    int x, y, width, height;
    std::unique_ptr<BspNode> left;
    std::unique_ptr<BspNode> right;
    Room* room = nullptr;  // 叶子节点才有房间
};

// 隐藏房间
struct HiddenRoom {
    Room room;
    int doorX = 0, doorY = 0;
    bool revealed = false;
    bool isPremium = false;   // 高级隐藏房间（需要钥匙+击败守护兽）
    int guardianIdx = -1;     // 守护兽在 m_enemies 中的索引（-1 表示无）
};

// 地牢地图——BSP 算法程序化生成
class DungeonMap {
public:
    static constexpr int MAP_WIDTH = 70;
    static constexpr int MAP_HEIGHT = 20;

    DungeonMap();

    // 生成指定层数的地牢
    void generate(int floor);

    // 渲染地图（可传入玩家指针以显示隐藏门提示）
    void render(Renderer& renderer, const Player* player = nullptr) const;

    // 坐标查询
    Tile getTile(int x, int y) const;
    bool isWalkable(int x, int y) const;
    bool isAtStairsDown(int x, int y) const;
    bool isAtStairsUp(int x, int y) const;
    bool isAtLockedStairs(int x, int y) const;
    void unlockStairs(int x, int y);
    int getStartX() const { return m_startX; }
    int getStartY() const { return m_startY; }
    int getHeight() const { return MAP_HEIGHT; }

    // 返回通知消息（空字符串表示无事件）
    std::string checkPlayerPosition(Player& player);

    // 获取当前层敌人列表
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return m_enemies; }
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return m_enemies; }
    void removeEnemy(int index);

    // NPC 交互
    std::vector<NPC>& getNPCs() { return m_npcs; }
    const std::vector<NPC>& getNPCs() const { return m_npcs; }
    int getNPCAt(int x, int y) const;

    // 房间信息（传送卷轴等需要）
    const std::vector<Room>& getRooms() const { return m_rooms; }

    // 隐藏房间
    bool isAdjacentToHiddenDoor(int px, int py, int& doorX, int& doorY) const;
    bool isPremiumHiddenDoor(int doorX, int doorY) const;    // 是否为高级隐藏门
    bool isGuardianAlive(int doorX, int doorY) const;        // 守护兽是否存活
    bool revealHiddenRoom(int doorX, int doorY, Player& player);
    const std::vector<HiddenRoom>& getHiddenRooms() const { return m_hiddenRooms; }

    // 深层怪物追击系统（6层+，怪物主动向玩家移动）
    void pursuePlayer(const Player& player);

private:
    // BSP 分割
    void bspSplit(BspNode* node, int depth, std::mt19937& rng);

    // 在叶子节点创建房间
    void createRooms(BspNode* node, std::mt19937& rng);

    // 连接房间（走廊）
    void connectRooms(const Room& a, const Room& b, std::mt19937& rng);

    // 放置实体
    void placeDownStairs(std::mt19937& rng);
    void placeUpStairs(std::mt19937& rng);
    void placeEnemies(int floor, std::mt19937& rng);
    void placeChests(std::mt19937& rng);
    void placeTraps(std::mt19937& rng);
    void placeNPCs(std::mt19937& rng);
    void placeHiddenRooms(std::mt19937& rng);

    // 清理
    void clearMap();

    // 地图数据
    std::vector<std::vector<Tile>> m_tiles;
    std::vector<Room> m_rooms;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<NPC> m_npcs;

    int m_startX = 0, m_startY = 0;
    int m_stairsDownX = 0, m_stairsDownY = 0;
    int m_stairsUpX = 0, m_stairsUpY = 0;
    int m_currentFloor = 1;

    std::vector<HiddenRoom> m_hiddenRooms;

    // 预计算的相邻隐藏门坐标（渲染用）
    mutable int m_adjacentHiddenDoorX = -1, m_adjacentHiddenDoorY = -1;
};
