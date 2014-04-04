

#include "mapgenerator.h"
#include <stdlib.h>
#include <vector>
#include <map>

#include "logging.h"

USING_NS_CC;

#define GENERATERANGE   (CHUNKWIDTH)
#define MAXROOMSIZE     5

MapGenerator::MapGenerator(GameMap* mp)
    : mGameMap(mp)
{
}

void MapGenerator::GenMapsNearChunk(GameMap::ChunkId id)
{
    if (id.x == 0 && id.y == 0) {
        makeRoom(-7, -7, 14, 14);
    }

    std::list<CCPoint> v;
    
    int i, j;
    // int index = 0;
    int centerX = id.getChunkCenterX();
    int centerY = id.getChunkCenterY();

    for (i = centerX - GENERATERANGE; i < centerY + GENERATERANGE; i++) {
        for (j = centerY - GENERATERANGE; j < centerY + GENERATERANGE; j++) {
            GameMap::Node* n = mGameMap->at(i, j);
            if (!n->isWall)
                continue;
            if (n->hasConsidered)
                continue;
            GameMap::Node* up = mGameMap->at(i, j + 1);
            GameMap::Node* down = mGameMap->at(i, j - 1);
            GameMap::Node* left = mGameMap->at(i - 1, j);
            GameMap::Node* right = mGameMap->at(i + 1, j);
            int wallcount = 0;
            if (up->isWall)
                wallcount++;
            if (down->isWall)
                wallcount++;
            if (left->isWall)
                wallcount++;
            if (right->isWall)
                wallcount++;
            if (wallcount == 3) {
                n->hasConsidered = true;
                CCPoint pt(i, j);
                v.push_back(pt);
                // RUN_HERE() << "consider point: " << pt;
            }
        }
    }

    while (!v.empty()) {
        // RUN_HERE() << "v.size() = " << v.size();
        int d = rand() % v.size();
        std::list<CCPoint>::iterator it = v.begin();
        for (int i = 0; i < d; i++)
            it++;
        CCPoint& pt = *it;
        CCSize sz = nextRoomSize();

        GameMap::Node* up = mGameMap->at(pt.x, pt.y + 1);
        GameMap::Node* down = mGameMap->at(pt.x, pt.y - 1);
        GameMap::Node* left = mGameMap->at(pt.x - 1, pt.y);
        // GameMap::Node* right = mGameMap->at(pt.x + 1, pt.y);
        
        if (!up->isWall) {
            int x = pt.x - sz.width / 2;
            int w = x + sz.width;
            int y = pt.y - sz.height + 1;
            int h = y + sz.height;
            CCRect  rect(x, y, w, h);
            if (hasEnoughRoom(rect)) {
                makeRoom(x, y, w, h);
            }
        } else if (!down->isWall) {
            int x = pt.x - sz.width / 2;
            int w = x + sz.width;
            int y = pt.y;
            int h = y + sz.height;
            CCRect  rect(x, y, w, h);
            if (hasEnoughRoom(rect)) {
                makeRoom(x, y, w, h);
            }
        } else if (!left->isWall) {
            int x = pt.x;
            int w = x + sz.width;
            int y = pt.y - sz.height / 2;
            int h = y + sz.height;
            CCRect  rect(x, y, w, h);
            if (hasEnoughRoom(rect)) {
                makeRoom(x, y, w, h);
            }
        } else {
            int x = pt.x - sz.width + 1;
            int w = x + sz.width;
            int y = pt.y - sz.height / 2;
            int h = y + sz.height;
            CCRect  rect(x, y, w, h);
            if (hasEnoughRoom(rect)) {
                makeRoom(x, y, w, h);
            }
        }
        v.erase(it);
    }
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
}

cocos2d::CCSize MapGenerator::nextRoomSize()
{
    int w = rand() % MAXROOMSIZE + 1;
    int h = rand() % MAXROOMSIZE + 1;
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





