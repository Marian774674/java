#pragma once
#include "../entity/Enemy.h"

class Player;

// 敌人 AI 策略——Strategy 模式
class EnemyAI {
public:
    virtual ~EnemyAI() = default;
    virtual Enemy::Action decide(const Enemy& enemy, const Player& player) = 0;
};
