
#pragma once

#include "creature.h"

class Monster : public Creature
{
public:
    static Monster* create();
};

