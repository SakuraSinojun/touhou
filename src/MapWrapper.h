
#pragma once

#include "cocos2d.h"
#include "MapTile.h"
#include "GameResource.h"

class ProjectileCallback
{
public:
    virtual void operator()() = 0;
};

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
    cocos2d::CCPoint mapToPoint(cocos2d::CCPoint point);

    bool addProjectile(cocos2d::CCNode* prj, cocos2d::CCPoint start, cocos2d::CCPoint end, float duration, ProjectileCallback* fp = NULL);

private:
    MapTile*                tiles[MAPWIDTH][MAPHEIGHT];
    cocos2d::CCSprite*      mGrid;
    bool                    mIsGridVisible;
    cocos2d::CCNode*        mProjectile;
    ProjectileCallback*     mProjectileCallback;
    void onProjectileMoveFinished(cocos2d::CCObject* pSender);
};


