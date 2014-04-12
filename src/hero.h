
#pragma once

#include "creature.h"
class Hero : public Creature
{
public:
    Hero();
    virtual ~Hero();

    static Hero* getInstance();

    virtual int attackRange() { return 5; }
    virtual void turn();

    virtual void StartWalkingAnimation(int dx, int dy);
    virtual void StopWalkingAnimation();

private:
};

