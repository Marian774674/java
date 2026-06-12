#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <map>
#include <string>

struct NPC;
struct SynthesisResult;
class Renderer;
class Player;
class DungeonMap;
class CombatSystem;
class SpellSystem;

// 游戏状态
enum class GameState {
    Exploring,   // 地牢探索
    InCombat,    // 战斗中
    Inventory,   // 背包/合成界面
    GameOver,    // 游戏结束
    Victory      // 胜利
};

// 结局类型
enum class EndingType {
    Normal,         // 段错误 — 默认
    Good,           // 编译成功 — >=2 任务
    TrueEnding,     // 真理之源 — 4任务 + 3残页 + >=1隐藏房间
    InfiniteLoop,   // 无限循环 — 逃跑>=5
    StackOverflow,  // 栈溢出 — 0法术 + 0任务
    HiddenEnding,   // 隐藏的真相 — >=3隐藏房间
    Completionist   // 完美清关 — 清空所有楼层的所有怪物
};

// 游戏引擎——Singleton，管理所有子系统和游戏主循环
class GameEngine {
public:
    static GameEngine& instance();

    // 禁止拷贝
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    // 初始化所有子系统
    void init();

    // 运行游戏主循环
    void run();

    // 状态切换
    void setState(GameState newState);
    GameState getState() const { return m_state; }

    // 子系统访问
    Renderer& getRenderer() { return *m_renderer; }
    Player& getPlayer() { return *m_player; }
    DungeonMap& getDungeon() { return *m_dungeon; }
    CombatSystem& getCombat() { return *m_combat; }
    SpellSystem& getSpellSystem() { return *m_spellSystem; }

    // 游戏数据
    int getCurrentFloor() const { return m_currentFloor; }
    void setCurrentFloor(int floor) { m_currentFloor = floor; }

    bool isRunning() const { return m_running; }
    void quit() { m_running = false; }

    // 清理
    void shutdown();

    // 当前帧最后一次按键
    int getLastKey() const { return m_lastKey; }

    // 通知系统
    void addNotification(const std::string& msg);
    const std::vector<std::string>& getNotifications() const { return m_notifications; }

public:
    GameEngine() = default;
    ~GameEngine();

    void renderExploring();
    void renderCombat();
    void renderInventory();
    void renderGameOver();

    void handleExploringInput();
    void handleCombatInput();
    void handleInventoryInput();
    void handleGameOverInput();

    void runForgeUI();
    void showSynthesisResult(const SynthesisResult& result);
    void runStoryScreen(const std::string& title, const std::vector<std::string>& lines,
                        bool waitForInput = true);
    bool showIntro();
    void showFloorStory();
    void showNPCDialogue(NPC& npc);
    void switchFloor(int targetFloor);
    EndingType determineEnding();
    void renderEndingScreen();

    GameState m_state = GameState::Exploring;
    EndingType m_endingType = EndingType::Normal;
    bool m_running = true;
    int m_currentFloor = 1;
    int m_lastKey = 0;
    int m_inventoryCursor = 0;  // 背包光标位置

    std::vector<std::string> m_notifications;
    static constexpr int MAX_NOTIFICATIONS = 6;

    // 楼层状态保存
    std::map<int, std::unique_ptr<DungeonMap>> m_floorMaps;
    std::map<int, int> m_savedPlayerX;
    std::map<int, int> m_savedPlayerY;

    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<DungeonMap> m_dungeon;
    std::unique_ptr<CombatSystem> m_combat;
    std::unique_ptr<SpellSystem> m_spellSystem;
};
