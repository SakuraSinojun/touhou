
#pragma once

#include "creature.h"
class Hero : public Creature
{
public:

    static Hero* getInstance();
private:
    Hero() {}
};

