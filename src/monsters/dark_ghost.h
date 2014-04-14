
#pragma once

#include "monster.h"
#include "cocos2d.h"
#include "ai/normal.h"

class DarkGhost : public Monster
{
public:
    DarkGhost() {
        cocos2d::CCRect  rect(0, 0, 32, 48);
        createSprite("monsters/darkghost.png", rect);

        mAI = new ai::Normal(this);
    }
    ~DarkGhost() {
        if (mAI) {
            delete mAI;
        }
    }
};

