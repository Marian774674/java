#include "Bug.h"

Bug::Bug(int x, int y, int floor)
    : Enemy(x, y, 'b', "Bug 小虫",
            20 + floor * 5,       // HP
            6 + floor * 2,        // ATK
            2 + floor,            // DEF
            EnemyType::Bug,
            30 + floor * 10)      // XP 奖励
{
    setLevel(floor);
}
