#include "Command.h"
#include "../entity/Player.h"
#include "../entity/Enemy.h"
#include <random>
#include <algorithm>

// === AttackCommand ===

std::string AttackCommand::execute(Player& player, Enemy& enemy) {
    // 玩家攻击敌人（普通攻击伤害降低至60%，法术是主要输出方式）
    m_damageDealt = enemy.takeDamage(player.dealDamage() * 3 / 5);
    std::string result = "(普通攻击) 你对 " + enemy.getName() + " 造成 " + std::to_string(m_damageDealt) + " 点伤害。";

    // 敌人还击
    if (enemy.isAlive()) {
        m_damageTaken = player.takeDamage(enemy.dealDamage());
        result += "\n" + enemy.getName() + " 反击造成 " + std::to_string(m_damageTaken) + " 点伤害。";
    }

    return result;
}

std::string AttackCommand::undo(Player& player, Enemy& enemy) {
    // 撤销：恢复血量
    enemy.heal(m_damageDealt);
    player.heal(m_damageTaken);
    return "撤销攻击。恢复了敌人 " + std::to_string(m_damageDealt) + " 点生命，恢复了你的 "
           + std::to_string(m_damageTaken) + " 点生命。";
}

// === DefendCommand ===

std::string DefendCommand::execute(Player& player, Enemy& enemy) {
    // 防御：伤害减半
    int rawDamage = enemy.dealDamage();
    m_blockedAmount = rawDamage / 2;
    int actualDamage = player.takeDamage(rawDamage / 2);

    return "进入防御姿态！格挡了 " + std::to_string(m_blockedAmount) + " 点伤害，受到 "
           + std::to_string(actualDamage) + " 点伤害。";
}

std::string DefendCommand::undo(Player& player, Enemy& enemy) {
    // 撤销防御：恢复受到的伤害
    player.heal(player.getMaxHp());  // 简化：只恢复防御减半的部分
    return "撤销了防御。";

    // 注意：这里简化处理，实际伤害 = m_blockedAmount
    (void)enemy;
}

// === FleeCommand ===

std::string FleeCommand::execute(Player& player, Enemy& enemy) {
    // 逃跑：成功率 70%（Boss 不可逃跑）
    if (!enemy.canFlee()) {
        m_succeeded = false;
        // 逃跑失败仍然受到攻击
        int dmg = player.takeDamage(enemy.dealDamage());
        return "Boss 无法逃跑！你受到 " + std::to_string(dmg) + " 点伤害。";
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    m_succeeded = (dist(rng) <= 70);

    if (m_succeeded) {
        return "成功逃跑！";
    } else {
        int dmg = player.takeDamage(enemy.dealDamage());
        return "逃跑失败！你受到 " + std::to_string(dmg) + " 点伤害。";
    }
}

std::string FleeCommand::undo(Player& player, Enemy& enemy) {
    // 逃跑不可撤销
    (void)player; (void)enemy;
    return "逃跑操作无法撤销。";
}
