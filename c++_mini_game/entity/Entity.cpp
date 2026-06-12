#include "Entity.h"
#include <algorithm>

Entity::Entity(int x, int y, char icon, std::string name, int maxHp, int atk, int def)
    : m_x(x), m_y(y)
    , m_icon(icon)
    , m_name(std::move(name))
    , m_hp(maxHp), m_maxHp(maxHp)
    , m_atk(atk)
    , m_def(def)
{}

int Entity::takeDamage(int rawDamage) {
    int actualDamage = std::max(1, rawDamage - m_def);
    m_hp = std::max(0, m_hp - actualDamage);
    return actualDamage;
}

int Entity::dealDamage() const {
    return m_atk;
}

void Entity::heal(int amount) {
    m_hp = std::min(m_maxHp, m_hp + amount);
}
