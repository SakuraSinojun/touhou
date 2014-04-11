
#pragma once

#include "creature.h"
class Hero : public Creature
{
public:
    static Hero* getInstance();

    virtual int attackRange() { return 5; }

private:
    Hero() {}
};

