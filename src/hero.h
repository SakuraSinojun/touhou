
#pragma once

#include "creature.h"
#include "MapWrapper.h"

class Hero : public Creature
{
public:
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

    virtual int sight();
    virtual int senseRange();

    virtual int maxHp();

private:
    class ProjectileHelper : public ProjectileCallback {
    public:
        virtual void operator()();
        MapLayer* gml;
        Creature* creature;
    };
    ProjectileHelper    mProjectileHelper;
};

