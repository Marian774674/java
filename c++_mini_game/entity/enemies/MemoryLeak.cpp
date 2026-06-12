#include "MemoryLeak.h"

MemoryLeak::MemoryLeak(int x, int y, int floor)
    : Enemy(x, y, 'M', "内存泄漏怪",
            30 + floor * 8,       // HP
            8 + floor * 2,        // ATK
            3 + floor,            // DEF
            EnemyType::MemoryLeak,
            50 + floor * 12)      // XP
{
    setLevel(floor);
}

int MemoryLeak::useSpecialSkill() {
    // 光环伤害随楼层增长: 5 + floor * 2
    return 5 + getLevel() * 2;
}

std::string MemoryLeak::getSpecialDescription() const {
    return "内存泄漏: 每回合造成 " + std::to_string(5 + getLevel() * 2) + " 点额外伤害";
}
