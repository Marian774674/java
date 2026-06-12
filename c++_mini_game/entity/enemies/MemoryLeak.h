#pragma once
#include "../Enemy.h"

// Memory Leak（内存泄漏怪）——持续掉血光环
class MemoryLeak : public Enemy {
public:
    MemoryLeak(int x, int y, int floor);

    int useSpecialSkill() override;
    std::string getSpecialDescription() const override;
};
