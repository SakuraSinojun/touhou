

#pragma once

#include "cocos2d.h"
#include "AppMacros.h"
#include "gamemap.h"

#include <list>


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
    void onEnsureMove();

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

    class PathGrid {
    public:
        PathGrid() : sprite(NULL) {}
        PathGrid(cocos2d::CCSprite* s, cocos2d::CCPoint p) : sprite(s), pt(p) {}
        cocos2d::CCSprite*  sprite;
        cocos2d::CCPoint    pt;
    };
    std::list<PathGrid> mPathGrids;
    void removeAllPathGrids();

private:
    class Direction {
    public:
        Direction() : dx(0), dy(0) {}
        Direction(int x, int y) : dx(x), dy(y) {}
        int x() {
            if (dx < 0) return -1;
            else if (dx > 0) return 1;
            else return 0; 
        }
        int y() {
            if (dy < 0) return -1;
            else if (dy > 0) return 1;
            else return 0; 
        }

    private:
        int dx;
        int dy;
    };

    void Walk(Direction direction);
    void onMoveFinished(cocos2d::CCObject* pSender);

    std::list<Direction>    mDirections;
public:
    int getMapLayerWidth() { return MAPWIDTH * 32; }
    int getMapLayerHeight() { return MAPHEIGHT * 32; }
};

