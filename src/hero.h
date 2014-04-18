
#pragma once

#include "creature.h"

class Hero : public Creature
{ public:
    Hero();
    virtual ~Hero();

    static void destroyHero();

    virtual int attackRange() { return 15; }

    /*
    virtual void onStartTurn(GameMapLayer* gml);
    virtual void onTurn(GameMapLayer* gml);
    virtual void onEndTurn(GameMapLayer* gml);
    */

    virtual bool attackAnimate(Creature& o, MapLayer* gml);
    virtual void StartWalkingAnimation(int dx, int dy);
    virtual void StopWalkingAnimation();
    virtual bool isHero() { return true; }

    virtual int sight();
    virtual int senseRange();

    virtual int maxHp();

};

