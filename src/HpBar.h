
#pragma once

#include "cocos2d.h"

class HpBar
{
public:
    HpBar();
    virtual ~HpBar();

    void setPercent(float per);

    cocos2d::CCSprite* getBar() { return mBackground; }

private:
    cocos2d::CCSprite* mBackground;
    cocos2d::CCSprite* mBar;
};

