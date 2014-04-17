
#pragma once

#include "monster.h"
#include "cocos2d.h"
#include "ai/normal.h"

class RedWiz : public Monster
{
public:
    RedWiz() {
        cocos2d::CCRect  rect(0, 0, 32, 48);
        createSprite("monsters/redwiz.png", rect);
        getSprite()->setScale(32.0f / 48.0f);
        mAI = new ai::Normal(this);
    }
    virtual ~RedWiz() {
        if (mAI) {
            delete mAI;
        }
    }

    virtual bool attackAnimate(Creature& o, MapLayer* gml) {
        return fireBall(o, gml);
    }

    virtual int attackRange() { return 7; }
};

