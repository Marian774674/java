#pragma once
#include "../Enemy.h"

// Stack Overflow（栈溢出巨兽）——高攻高血，Boss 级
class StackOverflow : public Enemy {
public:
    StackOverflow(int x, int y, int floor);

    int dealDamage() const override;
    std::string getSpecialDescription() const override;
};
