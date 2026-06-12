#pragma once
#include "Entity.h"
#include <memory>

// 怪物类型
enum class EnemyType {
    Bug,            // 小虫——普通
    MemoryLeak,     // 内存泄漏怪——光环扣血
    NullPointer,    // 空指针幽灵——概率闪避
    StackOverflow,  // 栈溢出巨兽——高攻高血
    Segfault        // Boss——段错误魔王
};

class EnemyAI;

// 怪物基类——所有敌人的抽象
class Enemy : public Entity {
public:
    Enemy(int x, int y, char icon, std::string name, int maxHp, int atk, int def,
          EnemyType type, int xpReward);

    ~Enemy() override;  // defined in .cpp where EnemyAI is complete

    EnemyType getEnemyType() const { return m_type; }
    int getXpReward() const { return m_xpReward; }

    // 怪物的 AI
    void setAI(std::unique_ptr<EnemyAI> ai);
    EnemyAI* getAI() const { return m_ai.get(); }

    // 决定行动（由 AI 策略执行）
    enum class Action { Attack, Defend, Special, None };
    virtual Action decideAction(const class Player& player);

    // 特殊技能（子类重写）
    virtual int useSpecialSkill() { return 0; }
    virtual std::string getSpecialDescription() const { return ""; }

    // 是否逃跑（Boss 不会逃跑）
    virtual bool canFlee() const { return m_type != EnemyType::Segfault; }

    // 钥匙持有者标记（每层只有一个敌人被标记）
    void setKeyHolder(bool v) { m_isKeyHolder = v; }
    bool isKeyHolder() const { return m_isKeyHolder; }

    // 闪避禁用标记（法术克制系统使用）
    void setEvasionDisabled(bool v) { m_evasionDisabled = v; }
    bool isEvasionDisabled() const { return m_evasionDisabled; }

    // 守护兽标记（高级隐藏房间的守护者）
    void setGuardian(bool v) { m_isGuardian = v; }
    bool isGuardian() const { return m_isGuardian; }

protected:
    EnemyType m_type;
    int m_xpReward;
    std::unique_ptr<EnemyAI> m_ai;
    bool m_isKeyHolder = false;
    bool m_evasionDisabled = false;
    bool m_isGuardian = false;
};
