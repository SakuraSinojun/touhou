
#pragma once

#include "cocos2d.h"
#include "gamemap.h"

class MapTile : public cocos2d::CCSprite
{
public:
    virtual bool init();

    CREATE_FUNC(MapTile);

    void setType(GameMap::NODETYPE type);

};

