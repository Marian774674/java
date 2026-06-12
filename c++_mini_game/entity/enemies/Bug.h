#pragma once
#include "../Enemy.h"

// Bug（小虫）——最常见的敌人
class Bug : public Enemy {
public:
    Bug(int x, int y, int floor);
};
