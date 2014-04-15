
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

    // bool moveCreature(Creature* c, cocos2d::CCPoint dest);
    // bool attackCreature(Creature* c1, Creature* c2);

private:
    MapWrapper*         mMapWrapper;
    cocos2d::CCPoint    mDestPoint;
    bool                mIsMoving;
    std::list<Creature*>    mActiveCreatures;
    std::list<cocos2d::CCPoint> mHeroPath;

    void registerWithTouchDispatcher();
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    cocos2d::CCPoint pointToMap(cocos2d::CCPoint point);
    
    void startMoving();
    bool moveBy1Grid();
    void onNextMoving(cocos2d::CCObject* pSender);

    void onTurn();
    void removeActiveCreature(Creature* c);
    void addActiveCreature(Creature* c);
};


