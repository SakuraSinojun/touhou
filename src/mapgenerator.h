
#pragma once

#include "gamemap.h"
#include "cocos2d.h"


class MapGenerator
{
public:
    MapGenerator(GameMap* mp);

    // near MapCenter;
    void GenMapsNearChunk(GameMap::ChunkId id);

private:
    void makeRoom(int x, int y, int w, int h);

    cocos2d::CCSize nextRoomSize();
    bool hasEnoughRoom(cocos2d::CCRect rect);

private:
    GameMap* mGameMap;
};

