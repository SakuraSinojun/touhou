
#pragma once

#include "cocos2d.h"
#include "MapTile.h"
#include "GameResource.h"
#include "logging.h"

class Creature;
class ProjectileCallback
{
public:
    ProjectileCallback() : mCreature(NULL) {}
    ProjectileCallback(Creature* c) : mCreature(c) {}
    virtual ~ProjectileCallback() {}

    void set(Creature* c) { mCreature = c; }
    virtual void operator()();
protected:
    Creature*   mCreature;
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
    void onProjectileMoveFinished(cocos2d::CCNode* pSender, void* data);
    class Projectile {
    public:
        Projectile() : mProjectile(NULL), mProjectileCallback(NULL) {}
        Projectile(cocos2d::CCNode* p, ProjectileCallback* cb) : mProjectile(p), mProjectileCallback(cb) {}
        cocos2d::CCNode*        mProjectile;
        ProjectileCallback*     mProjectileCallback;
    };

};


