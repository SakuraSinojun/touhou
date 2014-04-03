
#pragma once

#include "cocos2d.h"

class Creature
{
public:
    void Attach(cocos2d::CCSprite* cs);
    cocos2d::CCSprite* getSprite();

    cocos2d::CCSprite* createSprite();
    cocos2d::CCSprite* createSprite(const char* file);
    cocos2d::CCSprite* createSprite(const char * file, cocos2d::CCRect& rect);

public:
    void MoveTo(int x_, int y_) { x = x_, y = y_; }
    bool move(int dx, int dy) {
        x += dx;
        y += dy;
        return true;
    }
    int x;
    int y;

protected:
    cocos2d::CCSprite* mSprite;
};

