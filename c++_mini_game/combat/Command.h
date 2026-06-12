#pragma once
#include <string>
#include <memory>

class Player;
class Enemy;
class CombatSystem;

// Combat command base class - Command pattern
class CombatCommand {
public:
    virtual ~CombatCommand() = default;

    // 执行指令，返回描述文本
    virtual std::string execute(Player& player, Enemy& enemy) = 0;

    // 撤销指令（用于 undo）
    virtual std::string undo(Player& player, Enemy& enemy) = 0;

    // 指令名称
    virtual std::string getName() const = 0;
};

// 普通攻击指令
class AttackCommand : public CombatCommand {
public:
    std::string execute(Player& player, Enemy& enemy) override;
    std::string undo(Player& player, Enemy& enemy) override;
    std::string getName() const override { return "普通攻击"; }

private:
    int m_damageDealt = 0;
    int m_damageTaken = 0;
};

// 防御指令
class DefendCommand : public CombatCommand {
public:
    std::string execute(Player& player, Enemy& enemy) override;
    std::string undo(Player& player, Enemy& enemy) override;
    std::string getName() const override { return "防御"; }

private:
    int m_blockedAmount = 0;
};

// 逃跑指令（不能撤销）
class FleeCommand : public CombatCommand {
public:
    std::string execute(Player& player, Enemy& enemy) override;
    std::string undo(Player& player, Enemy& enemy) override;
    std::string getName() const override { return "逃跑"; }

    bool succeeded() const { return m_succeeded; }

private:
    bool m_succeeded = false;
};
