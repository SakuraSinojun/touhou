
#pragma once

#include "cocos2d.h"
#include <list>

class MapWrapper;
class Creature;
class MapLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(MapLayer);
    virtual bool init();

    MapWrapper*     wrapper() { return mMapWrapper; }
    void    onAttackFinished(Creature* attacker, Creature* c);

private:
    MapWrapper*         mMapWrapper;
    cocos2d::CCPoint    mDestPoint;
    bool                mIsMoving;
    bool                mIsAttacking;
    std::list<Creature*>    mActiveCreatures;
    std::list<cocos2d::CCPoint> mHeroPath;
    Creature*           mAttacker;

    void registerWithTouchDispatcher();
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    cocos2d::CCPoint pointToMap(cocos2d::CCPoint point);
    
    bool startAttacking(Creature* c);
    void startMoving();
    bool moveBy1Grid();
    void onNextMoving(cocos2d::CCObject* pSender);

    void onTurn();
    void removeActiveCreature(Creature* c);
    void addActiveCreature(Creature* c);
    void onCreatureDie(Creature* c, Creature* attacker);
    void pickUp(cocos2d::CCPoint point);
};


