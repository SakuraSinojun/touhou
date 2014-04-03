
#pragma once

#include "cocos2d.h"
#include "gamemap.h"

class Creature
{
public:
    void Attach(cocos2d::CCSprite* cs);
    cocos2d::CCSprite* getSprite();

    cocos2d::CCSprite* createSprite();
    cocos2d::CCSprite* createSprite(const char* file);
    cocos2d::CCSprite* createSprite(const char * file, cocos2d::CCRect& rect);

public:
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

    bool move(int dx, int dy, GameMap* mp) {
        MoveTo(x + dx, y + dy, mp);
        return true;
    }
    int x;
    int y;

protected:
    cocos2d::CCSprite* mSprite;
};

