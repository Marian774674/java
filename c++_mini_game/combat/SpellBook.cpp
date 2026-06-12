#include "SpellBook.h"

bool SpellBook::addSpell(const LearnedSpell& spell) {
    if (m_spells.size() >= MAX_SPELLS) {
        return false;
    }
    m_spells.push_back(spell);
    return true;
}
