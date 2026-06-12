#include "StackOverflow.h"

StackOverflow::StackOverflow(int x, int y, int floor)
    : Enemy(x, y, 'S', "栈溢出巨兽",
            50 + floor * 12,      // HP（高血量）
            12 + floor * 3,       // ATK（高攻击）
            5 + floor * 2,        // DEF（中防）
            EnemyType::StackOverflow,
            80 + floor * 20)      // XP
{
    setLevel(floor);
}

int StackOverflow::dealDamage() const {
    // 伤害随自身血量减少而增加（越战越强）
    // 愤怒系数随楼层增长: 0.5 + floor * 0.1
    double rageCoeff = 0.5 + getLevel() * 0.1;
    double rageMultiplier = 1.0 + (1.0 - static_cast<double>(m_hp) / m_maxHp) * rageCoeff;
    return static_cast<int>(m_atk * rageMultiplier);
}

std::string StackOverflow::getSpecialDescription() const {
    double rageCoeff = 0.5 + getLevel() * 0.1;
    double maxMult = 1.0 + rageCoeff;
    return "栈溢出: 生命值越低伤害越高 (最高 " + std::to_string(maxMult).substr(0, 4) + " 倍)";
}
