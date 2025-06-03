#ifndef TOWER_H
#define TOWER_H

#include <string>
#include "monster.h"

class Tower {
protected:
    int attackDamage;
    float attackSpeed;
    float range;
};

class ArcherTower : public Tower {
};

class CannonTower : public Tower {
};

#endif
