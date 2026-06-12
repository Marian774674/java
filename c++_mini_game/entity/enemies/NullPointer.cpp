#include "NullPointer.h"
#include <random>

NullPointer::NullPointer(int x, int y, int floor)
    : Enemy(x, y, 'N', "空指针幽灵",
            25 + floor * 6,       // HP
            10 + floor * 2,       // ATK
            1 + floor,            // DEF（低防，但闪避高）
            EnemyType::NullPointer,
            45 + floor * 12)      // XP
{
    setLevel(floor);
}

int NullPointer::takeDamage(int rawDamage) {
    // 如果闪避被法术克制禁用，直接受伤
    if (m_evasionDisabled) {
        return Enemy::takeDamage(rawDamage);
    }
    // 闪避率随楼层增长: 30% + floor*2%，上限 50%
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    int dodgeChance = 30 + getLevel() * 2;
    if (dodgeChance > 50) dodgeChance = 50;
    if (dist(rng) <= dodgeChance) {
        return 0;  // 闪避！
    }
    return Enemy::takeDamage(rawDamage);
}

std::string NullPointer::getSpecialDescription() const {
    int dodgeChance = 30 + getLevel() * 2;
    if (dodgeChance > 50) dodgeChance = 50;
    return "空指针解引用: " + std::to_string(dodgeChance) + "% 概率闪避攻击";
}
