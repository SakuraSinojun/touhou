
#pragma once

#include "AppMacros.h"
#include "gamemap.h"
#include "hero.h"

// class Hero;
// class GameMap;
class GameResource
{
public:
    static GameResource* getInstance();
    static void reset();
    ~GameResource();


    Hero* hero() { return mHero; }
    GameMap* gameMap() { return mGameMap; }

    typedef enum {
        CT_MOVE,
        CT_MAGIC,
    } CTACTION;
    CTACTION getAction() { return mAction; }
    void setAction(CTACTION act) { mAction = act; }

private:
    GameResource();
    Hero*       mHero;
    GameMap*    mGameMap;
    CTACTION    mAction;
};

#define TILESIZE    32
#define TILESIZEF   32.0f
#define MAPWIDTH    ((DESIGNWIDTH / TILESIZE) * 3)
#define MAPHEIGHT   ((DESIGNHEIGHT / TILESIZE) * 3)

#define HEROMOVEPERIOD   0.1f
#define MAPMOVEPERIOD    0.1f

