
#pragma once

#include "cocos2d.h"
#include "MapTile.h"
#include "GameResource.h"

class MapWrapper : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(MapWrapper);
    virtual bool init();

    void refresh();
    void showGrid(bool bShow = true);
    void useRedGrid();
    void useGreenGrid();
    void useYellowGrid();

    cocos2d::CCPoint mGridPosition;

private:
    MapTile* tiles[MAPWIDTH][MAPHEIGHT];
    cocos2d::CCSprite*  mGrid;
    bool mIsGridVisible;
};


