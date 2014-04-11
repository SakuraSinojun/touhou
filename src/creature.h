
#pragma once

#include "cocos2d.h"
#include "gamemap.h"
#include "logging.h"

class Creature
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
    int currentHp();
    int maxHp();

protected:
    cocos2d::CCSprite* mSprite;

    int mCurrentHP;
};

