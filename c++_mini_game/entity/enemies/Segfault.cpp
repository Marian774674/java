#include "Segfault.h"
#include "../Player.h"

Segfault::Segfault(int x, int y, int floor)
    : Enemy(x, y, 'X', "Segfault 魔王",
            100 + floor * 20,     // HP（极高）
            15 + floor * 4,       // ATK
            8 + floor * 3,        // DEF
            EnemyType::Segfault,
            200 + floor * 50)     // XP（大量经验）
{
    setLevel(floor);
}

Enemy::Action Segfault::decideAction(const Player& player) {
    ++m_phaseCounter;

    // 每3回合释放一次毁灭性大招
    if (m_phaseCounter >= 3) {
        m_phaseCounter = 0;
        return Action::Special;
    }

    // 血量低于30%时倾向防御
    if (m_hp < m_maxHp * 0.3) {
        return Action::Defend;
    }

    return Action::Attack;
}

int Segfault::useSpecialSkill() {
    // 核心转储伤害随楼层增长: 30 + floor * 5
    return 30 + getLevel() * 5;
}

std::string Segfault::getSpecialDescription() const {
    return "核心转储: 每 3 回合释放一次毁灭性攻击 (" +
           std::to_string(30 + getLevel() * 5) + " 点伤害)";
}
