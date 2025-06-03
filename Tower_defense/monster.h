#ifndef MONSTER_H
#define MONSTER_H

#include <string>

class Monster {
protected:
    int hp, defense;
    float moveSpeed;

public:
    
};

class TankMonster : public Monster {
};

class FastMonster : public Monster {
};

#endif 
