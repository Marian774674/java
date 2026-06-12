#include "Enemy.h"
#include "../ai/EnemyAI.h"

Enemy::Enemy(int x, int y, char icon, std::string name, int maxHp, int atk, int def,
             EnemyType type, int xpReward)
    : Entity(x, y, icon, std::move(name), maxHp, atk, def)
    , m_type(type)
    , m_xpReward(xpReward)
{}

Enemy::~Enemy() = default;  // must be here where EnemyAI is complete

void Enemy::setAI(std::unique_ptr<EnemyAI> ai) {
    m_ai = std::move(ai);
}

Enemy::Action Enemy::decideAction(const Player& player) {
    if (m_ai) {
        return m_ai->decide(*this, player);
    }
    // 默认：简单攻击
    return Action::Attack;
}
