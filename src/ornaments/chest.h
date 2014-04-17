
#pragma once

#include "ornament.h"

class Chest : public Ornament
{
public:
    Chest() {
        canpass = false;
        blocksight = false;

        cocos2d::CCRect rect(0, 0, 32, 32);
        createSprite("maps/chest.png", rect);
    }
    virtual ~Chest() {}
};

