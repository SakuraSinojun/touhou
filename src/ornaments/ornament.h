
#pragma once

#include "cocos2d.h"


class GameMap;
class Ornament
{
public:
    Ornament();
    virtual ~Ornament();

    static Ornament* create();

    cocos2d::CCSprite* Sprite();
    cocos2d::CCSprite* createSprite();
    cocos2d::CCSprite* createSprite(const char* file);
    cocos2d::CCSprite* createSprite(const char * file, cocos2d::CCRect& rect);

protected:
    friend class GameMap;
    bool canpass;
    bool blocksight;

    cocos2d::CCSprite* mSprite;
};

