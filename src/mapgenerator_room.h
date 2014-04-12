
#pragma once

#include "mapgenerator.h"
#include "gamemap.h"
#include "cocos2d.h"

#include <list>

class MapGeneratorRoom : public MapGenerator
{
public:
    MapGeneratorRoom(GameMap* mp);

    // near MapCenter;
    virtual void GenMapsNearChunk(GameMap::ChunkId id);

    virtual GameMap::BIOME getBiome(int x, int y);

private:
    void makeRoom(int x, int y, int w, int h);
    void makeDoor(int x, int y);

    cocos2d::CCSize nextRoomSize();
    bool hasEnoughRoom(cocos2d::CCRect rect);

    class Wall {
    public:
        typedef enum {
            D_NORTH,
            D_SOUTH,
            D_WEST,
            D_EAST,
        } DIRECTION;

        Wall() : considered(false), direction(D_NORTH) {}
        Wall(int xx1, int yy1, int xx2, int yy2) : x1(xx1), y1(yy1), x2(xx2), y2(yy2), considered(false), direction(D_NORTH) {}

        void set(int xx1, int yy1, int xx2, int yy2) {
            x1 = xx1; y1 = yy1; x2 = xx2; y2 = yy2;
        }

        bool intersects(const Wall& o);
        bool contains(const Wall& o);
        Wall operator-(const Wall& o);

        int getMinX() const { return (x1 < x2) ? x1 : x2; }
        int getMaxX() const { return (x1 < x2) ? x2 : x1; }
        int getMinY() const { return (y1 < y2) ? y1 : y2; }
        int getMaxY() const { return (y1 < y2) ? y2 : y1; }
        int width() { return getMaxX() - getMinX() + 1; }
        int height() { return getMaxY() - getMinY() + 1; }

        int x1, y1, x2, y2;
        bool considered;
        DIRECTION direction;
    };
    std::list<Wall> mWalls;

    bool checkAndMakeWall(Wall& w);

    GameMap::BIOME getRawBiome(int x, int y);
    bool first_room_gened;
};

