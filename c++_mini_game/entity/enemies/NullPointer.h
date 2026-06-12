#pragma once
#include "../Enemy.h"

// Null Pointer（空指针幽灵）——概率闪避
class NullPointer : public Enemy {
public:
    NullPointer(int x, int y, int floor);

    int takeDamage(int rawDamage) override;
    std::string getSpecialDescription() const override;
};
