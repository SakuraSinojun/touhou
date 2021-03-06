
#pragma once

#include "cocos2d.h"
#include "gamemap.h"
#include "logging.h"
#include "HpBar.h"
#include "MapWrapper.h"

#include "ai/ai.h"
#include "common/weak_ptr.h"
#include <string>

class MapLayer;
class Item;
class Creature : public SupportsWeakPtr<Creature>, public cocos2d::CCObject
{
public:
    // constructor
    Creature();
    virtual ~Creature();

public:
    // cocos
    void Attach(cocos2d::CCSprite* cs);
    cocos2d::CCSprite* getSprite();

    cocos2d::CCSprite* createSprite();
    cocos2d::CCSprite* createSprite(const char* file);
    cocos2d::CCSprite* createSprite(const char * file, cocos2d::CCRect& rect);

    cocos2d::CCSprite* getBar() { return mHpBar->getBar(); }
    // cocos2d::CCSprite* getHpBar() { return mHpBar; }

    virtual void StartWalkingAnimation(int dx, int dy);
    virtual void StopWalkingAnimation();

    virtual bool isHero() { return false; }

    virtual bool findPathAndMove(int dx, int dy);
    virtual bool move(int dx, int dy, GameMap* mp);
    virtual bool attack(Creature& o, MapLayer* gml);
    virtual bool attack(Creature* o, MapLayer* gml);
    virtual bool attackAnimate(Creature& o, MapLayer* gml);

    virtual void onAttackFinished(cocos2d::CCObject* pSender);
    std::string name() { return mName; }

    std::list<Item*>    items;
protected:
    virtual bool fireBall(Creature&o, MapLayer* gml);

public:
    // moving
    void MoveTo(int x_, int y_, GameMap* mp) {
        if (mp) {
            mp->at(x, y)->creature = NULL;
            x = x_;
            y = y_;
            mp->at(x, y)->creature = this;
        }
    }
    void MoveTo(int x_, int y_, GameMap& mp) {
        MoveTo(x_, y_, &mp);
    }

    int x;
    int y;

public:
    // character
    virtual int currentHp();
    virtual int maxHp();
    virtual int attackRange();


    int& speed() { return mSpeed; }
    void resetSpeed() { mSpeed = mMaxSpeed; }
    int maxSpeed() { return mMaxSpeed; }

    virtual int sight();
    virtual int senseRange();

    int expOnDeath();
    int exp();
    int addExp(int d);
    int level();

    // 
    virtual void onTurn(MapLayer* gml);

protected:
    cocos2d::CCSprite*  mSprite;
    HpBar*              mHpBar;
    MapLayer*           mMapLayer;
    Creature*           mTarget;
    std::string         mName;

    int mCurrentHP;
    
    ai::AI*     mAI;
    int mMaxSpeed;
    int mSpeed;
    int mLevel;
    int mExp;

    ProjectileCallback mProjectileHelper;
};

