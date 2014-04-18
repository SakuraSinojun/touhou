
#pragma once

#include "cocos2d.h"


class Item {
public:
    Item();
    virtual ~Item();

    cocos2d::CCSprite* Sprite();
    cocos2d::CCSprite* createSprite();
    cocos2d::CCSprite* createSprite(const char* file);
    cocos2d::CCSprite* createSprite(const char * file, cocos2d::CCRect& rect);

    static Item* create();

protected:
    cocos2d::CCSprite* mSprite;
    void assignTag();
};

