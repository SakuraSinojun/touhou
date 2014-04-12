
#pragma once

#include "gamemap.h"

class MapGenerator
{
public:
    MapGenerator(GameMap* mp) : mGameMap(mp) {}
    virtual ~MapGenerator() {}

    // near MapCenter;
    virtual void GenMapsNearChunk(GameMap::ChunkId id) = 0;
    virtual GameMap::BIOME getBiome(int x, int y) = 0;

protected:
    GameMap* mGameMap;
};

