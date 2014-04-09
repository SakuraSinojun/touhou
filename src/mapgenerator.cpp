

#include "mapgenerator.h"
#include <stdlib.h>
#include <vector>
#include <map>

#include "logging.h"

USING_NS_CC;

#define GENERATERANGE   (CHUNKWIDTH * 5)
#define MAXROOMSIZE     20
#define MINROOMSIZE     5 

MapGenerator::MapGenerator(GameMap* mp)
    : mGameMap(mp)
{
}

void MapGenerator::GenMapsNearChunk(GameMap::ChunkId id)
{
    if (id.x == 0 && id.y == 0) {
        makeRoom(-7, -7, 14, 14);
    }

    int centerX = id.getChunkCenterX();
    int centerY = id.getChunkCenterY();

    RUN_HERE() << "unused walls = " << mWalls.size();

    std::list<Wall>::iterator it = mWalls.begin();
    while (it != mWalls.end()) {
        Wall& w = *it;
        if (w.considered) {
            it++;
            continue;
        }

        Wall temp(centerX - GENERATERANGE, centerY - GENERATERANGE, centerX + GENERATERANGE, centerY + GENERATERANGE);
        if (!temp.intersects(w)) {
            it++;
            continue;
        }


        w.considered = true;

        CCSize sz = nextRoomSize();
        CCRect rect;
        CCPoint door;
        if (w.direction == Wall::D_NORTH) {
            int door1 = rand() % w.width();
            int door2 = rand() % (int)sz.width;
            rect.setRect(w.getMinX() + door1 - door2, w.getMaxY() + 1, sz.width, sz.height);
            door.setPoint(w.getMinX() + door1, w.getMaxY());
        } else if (w.direction == Wall::D_SOUTH) {
            int door1 = rand() % w.width();
            int door2 = rand() % (int)sz.width;
            rect.setRect(w.getMinX() + door1 - door2, w.getMinY() - sz.height, sz.width, sz.height);
            door.setPoint(w.getMinX() + door1, w.getMinY());
        } else if (w.direction == Wall::D_WEST) {
            int door1 = rand() % w.height();
            int door2 = rand() % (int)sz.height;
            rect.setRect(w.getMinX() - sz.width, w.getMinY() + door1 - door2, sz.width, sz.height);
            door.setPoint(w.getMinX(), w.getMinY() + door1);
        } else {
            int door1 = rand() % w.height();
            int door2 = rand() % (int)sz.height;
            rect.setRect(w.getMinX() + 1, w.getMinY() + door1 - door2, sz.width, sz.height);
            door.setPoint(w.getMaxX(), w.getMinY() + door1);
        }
        if (hasEnoughRoom(rect)) {
            makeRoom(rect.getMinX(), rect.getMinY(), rect.size.width, rect.size.height);
            makeDoor(door.x, door.y);
        }
        it = mWalls.erase(it);
    }

}

void MapGenerator::makeDoor(int x, int y)
{
    GameMap::Node* n = mGameMap->at(x, y);
    n->type = GameMap::NODE_GRASS;
    n->canpass = true;
    n->blocksight = false;
    n->isWall = false;
}

void MapGenerator::makeRoom(int x, int y, int w, int h)
{
    int i, j;
    for (i = x; i < x + w; i++) {
        for (j = y; j < y + h; j++) {
            GameMap::Node* n = mGameMap->at(i, j);
            n->type = GameMap::NODE_GRASS;
            n->canpass = true;
            n->blocksight = false;
            n->isWall = false;
        }
    }

#if 0
    // make walls
    // {{{
    for (i = x; i < x + w; i++) {
        int y1 = y - 1;
        int y2 = y + h;
        GameMap::Node* n1 = mGameMap->at(i, y1);
        GameMap::Node* n2 = mGameMap->at(i, y2);
        if (n1->type == GameMap::NODE_DIRT) {
            n1->type = GameMap::NODE_TREE;
            n1->canpass = false;
            n1->blocksight = true;
            n1->isWall = true;
        }
        if (n2->type == GameMap::NODE_DIRT) {
            n2->type = GameMap::NODE_TREE;
            n2->canpass = false;
            n2->blocksight = true;
            n2->isWall = true;
        }
    }
    for (j = y; j < y + h; j++) {
        int x1 = x - 1;
        int x2 = x + w;
        GameMap::Node* n1 = mGameMap->at(x1, j);
        GameMap::Node* n2 = mGameMap->at(x2, j);
        if (n1->type == GameMap::NODE_DIRT) {
            n1->type = GameMap::NODE_TREE;
            n1->canpass = false;
            n1->blocksight = true;
            n1->isWall = true;
        }
        if (n2->type == GameMap::NODE_DIRT) {
            n2->type = GameMap::NODE_TREE;
            n2->canpass = false;
            n2->blocksight = true;
            n2->isWall = true;
        }
    }
    // }}}
#endif

    Wall wall(x, y - 1, x + w - 1, y - 1);
    if (w > 1 && checkAndMakeWall(wall)) {
        wall.direction = Wall::D_SOUTH;
        mWalls.push_back(wall);
    }
    wall.set(x, y + h, x + w - 1, y + h);
    if (w > 1 && checkAndMakeWall(wall)) {
        wall.direction = Wall::D_NORTH;
        mWalls.push_back(wall);
    }
    wall.set(x - 1, y, x - 1, y + h - 1);
    if (h > 1 && checkAndMakeWall(wall)) {
        wall.direction = Wall::D_WEST;
        mWalls.push_back(wall);
    }
    wall.set(x + w, y, x + w, y + h - 1);
    if (h > 1 && checkAndMakeWall(wall)) {
        wall.direction = Wall::D_EAST;
        mWalls.push_back(wall);
    }
}

cocos2d::CCSize MapGenerator::nextRoomSize()
{
    int w = 0, h = 0;
    int type = rand() % 10;
    if (type < 4) {
        // passage
        if (type < 2) {
            w = 1;
            h = rand() % (MAXROOMSIZE - MINROOMSIZE) + 1 + MINROOMSIZE;
        } else {
            w = rand() % (MAXROOMSIZE - MINROOMSIZE) + 1 + MINROOMSIZE;
            h = 1;
        }
    } else {
        // ROOM
        w = rand() % (MAXROOMSIZE - MINROOMSIZE) + 1 + MINROOMSIZE;
        h = rand() % (MAXROOMSIZE - MINROOMSIZE) + 1 + MINROOMSIZE;
    }
    return CCSize(w, h);
}

bool MapGenerator::hasEnoughRoom(cocos2d::CCRect rect)
{
    int i, j;
    for (i = rect.getMinX(); i < rect.getMaxX(); i++) {
        for (j = rect.getMinY(); j < rect.getMaxY(); j++) {
            GameMap::Node* n = mGameMap->at(i, j);
            if (!n->isWall)
                return false;
        }
    }
    return true;
}



bool MapGenerator::checkAndMakeWall(Wall& w)
{
    std::list<Wall>::iterator it;
    for (it = mWalls.begin(); it != mWalls.end(); it++) {
        Wall& o = *it;
        if (o.contains(w))
            return false;
        else if (o.intersects(w)) {
            w = w - o;
            return true;
        }
    }
    return true;
}



bool MapGenerator::Wall::intersects(const MapGenerator::Wall& o)
{
    return !(getMaxX() < o.getMinX() || getMinX() > o.getMaxX() || getMaxY() < o.getMinY() || getMinY() > o.getMaxY());
}

bool MapGenerator::Wall::contains(const MapGenerator::Wall& o)
{
    return (getMinX() <= o.getMinX() && getMaxX() >= o.getMaxX() && getMinY() <= o.getMinY() && getMaxY() >= o.getMaxY());
}

MapGenerator::Wall MapGenerator::Wall::operator-(const Wall& o)
{
    // temporary
    return *this;
}



