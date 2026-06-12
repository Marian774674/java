#pragma once
#include "EnemyAI.h"
#include <random>

// 激进型 AI——偏重攻击
class AggressiveAI : public EnemyAI {
public:
    Enemy::Action decide(const Enemy& enemy, const Player& player) override;
};

// 防御型 AI——偏重防御和特殊技能
class DefensiveAI : public EnemyAI {
public:
    Enemy::Action decide(const Enemy& enemy, const Player& player) override;
};

// 随机型 AI——不可预测
class RandomAI : public EnemyAI {
public:
    Enemy::Action decide(const Enemy& enemy, const Player& player) override;
};
