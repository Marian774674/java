#pragma once
#include "../Enemy.h"

// Segfault（段错误魔王）——最终 Boss
class Segfault : public Enemy {
public:
    Segfault(int x, int y, int floor);

    Action decideAction(const class Player& player) override;
    int useSpecialSkill() override;
    std::string getSpecialDescription() const override;
    bool canFlee() const override { return false; }  // Boss 不能逃跑

private:
    int m_phaseCounter = 0;  // 阶段计数器（每3回合释放大招）
};
