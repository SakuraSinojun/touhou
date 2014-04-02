
#pragma once

#include "cocos2d.h"

class Creature
{
public:
    
    void Attach(cocos2d::CCSprite* cs);
    cocos2d::CCSprite* getSprite();

protected:
    cocos2d::CCSprite* mSprite;
};

