
#pragma once

#include "cocos2d.h"
#include "gamemap.h"
#include "logging.h"
#include "HpBar.h"

#include "ai/ai.h"
#include "common/weak_ptr.h"

class GameMapLayer;
class Creature : public SupportsWeakPtr<Creature>
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

public:
    // moving
    void MoveTo(int x_, int y_, GameMap* mp) {
        if (mp) {
            mp->at(x, y)->creature = NULL;
            x = x_;
            y = y_;
            mp->at(x, y)->creature = this;
            // if (x == 0 && y == 0) RUN_HERE() << "(" << x << ", " << y << ") " << mp->at(x, y)->creature;
            // RUN_HERE() << "(" << x << ", " << y << ") " << mp->at(x, y)->creature;
        }
    }
    void MoveTo(int x_, int y_, GameMap& mp) {
        MoveTo(x_, y_, &mp);
    }

    bool move(int dx, int dy, GameMap* mp) {
        MoveTo(x + dx, y + dy, mp);
        return true;
    }
    int x;
    int y;

public:
    // character
    virtual int currentHp();
    virtual int maxHp();
    virtual int attackRange();

    virtual Creature& attack(Creature& o);
    virtual Creature& attack(Creature* o);

    virtual int speed();
    virtual int sight();
    virtual int senseRange();

    // 
    virtual void onTurn(GameMapLayer* gml);
    virtual void onEndTurn(GameMapLayer* gml);

protected:
    cocos2d::CCSprite*  mSprite;
    HpBar*              mHpBar;

    int mCurrentHP;
    
    ai::AI*     mAI;
};

