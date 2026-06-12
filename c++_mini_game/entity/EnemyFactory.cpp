#include "EnemyFactory.h"
#include "enemies/Bug.h"
#include "enemies/MemoryLeak.h"
#include "enemies/NullPointer.h"
#include "enemies/StackOverflow.h"
#include "enemies/Segfault.h"
#include "../ai/EnemyAI.h"
#include "../ai/AITypes.h"

EnemyFactory& EnemyFactory::instance() {
    static EnemyFactory factory;
    return factory;
}

int EnemyFactory::scaleStat(int base, int floor) const {
    // 更深层怪物更强（0.5 倍增长系数）
    return static_cast<int>(base * (1.0 + (floor - 1) * 0.5));
}

std::unique_ptr<Enemy> EnemyFactory::createRandom(int floor, int x, int y, std::mt19937& rng) {
    // 根据层数决定可生成的怪物类型
    std::vector<EnemyType> available = {EnemyType::Bug};

    if (floor >= 2) available.push_back(EnemyType::MemoryLeak);
    if (floor >= 2) available.push_back(EnemyType::NullPointer);
    if (floor >= 3) available.push_back(EnemyType::StackOverflow);
    if (floor >= 6) available.push_back(EnemyType::StackOverflow);  // 深层 StackOverflow 权重翻倍
    if (floor >= 8) available.push_back(EnemyType::Segfault);       // 深层出现 Segfault 作为普通怪

    std::uniform_int_distribution<int> dist(0, available.size() - 1);
    EnemyType type = available[dist(rng)];

    std::unique_ptr<Enemy> enemy;
    switch (type) {
        case EnemyType::Bug:
            enemy = std::make_unique<Bug>(x, y, floor);
            break;
        case EnemyType::MemoryLeak:
            enemy = std::make_unique<MemoryLeak>(x, y, floor);
            break;
        case EnemyType::NullPointer:
            enemy = std::make_unique<NullPointer>(x, y, floor);
            break;
        case EnemyType::StackOverflow:
            enemy = std::make_unique<StackOverflow>(x, y, floor);
            break;
        default:
            enemy = std::make_unique<Bug>(x, y, floor);
            break;
    }

    // 设置 AI
    switch (type) {
        case EnemyType::Bug:
            enemy->setAI(std::make_unique<AggressiveAI>());
            break;
        case EnemyType::MemoryLeak:
            enemy->setAI(std::make_unique<DefensiveAI>());
            break;
        case EnemyType::NullPointer:
            enemy->setAI(std::make_unique<RandomAI>());
            break;
        case EnemyType::StackOverflow:
            enemy->setAI(std::make_unique<AggressiveAI>());
            break;
        case EnemyType::Segfault:
            enemy->setAI(std::make_unique<AggressiveAI>());
            break;
        default:
            break;
    }

    return enemy;
}

std::unique_ptr<Enemy> EnemyFactory::createBoss(int floor, int x, int y) {
    auto boss = std::make_unique<Segfault>(x, y, floor);
    boss->setAI(std::make_unique<AggressiveAI>());
    return boss;
}
