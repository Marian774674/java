#include "AITypes.h"
#include "../entity/Player.h"

Enemy::Action AggressiveAI::decide(const Enemy& enemy, const Player& player) {
    // 70% 攻击，20% 特殊技能，10% 防御
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    int roll = dist(rng);

    if (roll <= 70) return Enemy::Action::Attack;
    if (roll <= 90) return Enemy::Action::Special;
    return Enemy::Action::Defend;
}

Enemy::Action DefensiveAI::decide(const Enemy& enemy, const Player& player) {
    // 血量低于 40% 时优先防御
    if (enemy.getHp() < enemy.getMaxHp() * 0.4) {
        return Enemy::Action::Defend;
    }
    // 50% 攻击，30% 防御，20% 特殊
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    int roll = dist(rng);

    if (roll <= 50) return Enemy::Action::Attack;
    if (roll <= 80) return Enemy::Action::Defend;
    return Enemy::Action::Special;
}

Enemy::Action RandomAI::decide(const Enemy& enemy, const Player& player) {
    // 完全随机——不可预测的混沌
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 2);
    switch (dist(rng)) {
        case 0: return Enemy::Action::Attack;
        case 1: return Enemy::Action::Defend;
        default: return Enemy::Action::Special;
    }
}
