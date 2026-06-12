#pragma once
#include <string>

// 实体基类——所有游戏中可交互对象的抽象
class Entity {
public:
    Entity(int x, int y, char icon, std::string name, int maxHp, int atk, int def);

    virtual ~Entity() = default;

    // 基础属性访问
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setPosition(int x, int y) { m_x = x; m_y = y; }

    char getIcon() const { return m_icon; }
    std::string getName() const { return m_name; }

    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
    bool isAlive() const { return m_hp > 0; }

    int getAtk() const { return m_atk; }
    int getDef() const { return m_def; }

    // 战斗相关
    virtual int takeDamage(int rawDamage);
    virtual int dealDamage() const;
    void heal(int amount);

    // 等级相关
    int getLevel() const { return m_level; }
    void setLevel(int level) { m_level = level; }

protected:
    int m_x, m_y;
    char m_icon;
    std::string m_name;
    int m_hp, m_maxHp;
    int m_atk;
    int m_def;
    int m_level = 1;
};
