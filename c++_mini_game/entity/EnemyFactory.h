#pragma once
#include "Enemy.h"
#include <memory>
#include <random>

// 怪物工厂——Singleton + 模板方法，按层数参数化生成怪物
class EnemyFactory {
public:
    static EnemyFactory& instance();

    // 随机生成一个怪物（根据层数和位置）
    std::unique_ptr<Enemy> createRandom(int floor, int x, int y, std::mt19937& rng);

    // 生成 Boss
    std::unique_ptr<Enemy> createBoss(int floor, int x, int y);

    // 模板方法：按类型创建
    template<typename T, typename... Args>
    std::unique_ptr<T> create(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

private:
    EnemyFactory() = default;

    // 属性随层数缩放
    int scaleStat(int base, int floor) const;
};
