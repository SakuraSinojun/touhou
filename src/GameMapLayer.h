

#pragma once

#include "cocos2d.h"
#include "AppMacros.h"
#include "gamemap.h"


class MapTile;
class GameMapLayer : public  cocos2d::CCLayer
{
public:
    CREATE_FUNC(GameMapLayer)
    virtual bool init();


    void centerMap(cocos2d::CCPoint point);
    cocos2d::CCPoint getMapCenter();

    template <class U>
    class Helper : public GameMap::FpCallbackFunctor {
    public:
        Helper(U& f) : fp(f) {}
        virtual void operator() (int dx, int dy) {
            fp(dx, dy);
        }
        U& fp;
    };

    template <class CallbackFunctor>
    bool findPath(cocos2d::CCPoint pt1, cocos2d::CCPoint pt2, CallbackFunctor& fp) {
        Helper<CallbackFunctor>  h(fp);
        return mGameMap.findPath(pt1.x, pt1.y, pt2.x, pt2.y, h);
    }

    GameMap& gamemap() { return mGameMap; }


    void onClick(cocos2d::CCPoint point);

private:
    void registerWithTouchDispatcher();
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    cocos2d::CCPoint pointToMap(cocos2d::CCPoint point);
    cocos2d::CCRect  gridToRect(cocos2d::CCPoint grid);
    cocos2d::CCPoint gridToPoint(cocos2d::CCPoint grid);
    cocos2d::CCPoint gridToMap(cocos2d::CCPoint grid);
    cocos2d::CCPoint mapToGrid(cocos2d::CCPoint pMap);


#define MAPWIDTH    ((DESIGNWIDTH / 32) + 2)
#define MAPHEIGHT   ((DESIGNHEIGHT / 32) + 2)

    MapTile* tiles[MAPWIDTH][MAPHEIGHT];

    void refreshMap();

    GameMap             mGameMap;

    cocos2d::CCSprite*  mGrid;
    cocos2d::CCPoint    mGridPosition;

public:
    int getMapLayerWidth() { return MAPWIDTH * 32; }
    int getMapLayerHeight() { return MAPHEIGHT * 32; }
};

