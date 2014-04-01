

#pragma once

#include "cocos2d.h"
#include "AppMacros.h"
#include "gamemap.h"


class MapTile;
class GameMapLayer : public  cocos2d::CCLayer
{
public:
    virtual bool init();

    CREATE_FUNC(GameMapLayer);


    void at(cocos2d::CCPoint point);

    void centerMap(cocos2d::CCPoint point);

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

private:
    void registerWithTouchDispatcher();
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

#define MAPWIDTH    ((DESIGNWIDTH / 32) + 2)
#define MAPHEIGHT   ((DESIGNHEIGHT / 32) + 2)

    MapTile* tiles[MAPWIDTH][MAPHEIGHT];

    void refreshMap();

    cocos2d::CCPoint    mCurrentPoint;
    GameMap             mGameMap;
};

