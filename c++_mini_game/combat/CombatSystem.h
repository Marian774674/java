#pragma once
#include <vector>
#include <stack>
#include <memory>
#include <string>

class Player;
class Enemy;
class CombatCommand;
class GameEngine;

// 战斗结果
enum class CombatResult {
    Ongoing,      // 战斗中
    PlayerWin,    // 玩家胜利
    PlayerLost,   // 玩家失败
    PlayerFled    // 玩家逃跑
};

// 战斗系统——管理回合制战斗流程
class CombatSystem {
public:
    CombatSystem();
    ~CombatSystem();  // defined in .cpp where CombatCommand is complete

    // 开始战斗
    void startCombat(Player& player, Enemy& enemy);

    // 渲染战斗界面到缓冲区
    void render(GameEngine& engine);

    // 处理按键输入
    void handleInput(GameEngine& engine, int key);

    // 当前状态
    CombatResult getResult() const { return m_result; }
    bool isActive() const { return m_result == CombatResult::Ongoing; }

    // 指令历史（undo 用）
    void pushCommand(std::unique_ptr<CombatCommand> cmd);
    bool canUndo() const { return !m_commandHistory.empty(); }
    std::string undoLastCommand();

    // 战斗日志
    const std::vector<std::string>& getBattleLog() const { return m_battleLog; }

private:
    void processPlayerTurn(GameEngine& engine);
    void processEnemyTurn();
    void checkCombatEnd();

    Player* m_player = nullptr;
    Enemy* m_enemy = nullptr;
    CombatResult m_result = CombatResult::Ongoing;

    std::stack<std::unique_ptr<CombatCommand>> m_commandHistory;
    std::vector<std::string> m_battleLog;

    bool m_playerDefending = false;  // 玩家是否在防御
    int m_turnCount = 0;
};
