

#include "gamemap.h"

#include "dice.h"
#include <algorithm>
#include <list>
#include <math.h>
#include <vector>
#include "mapgenerator.h"
#include "mapgenerator_room.h"
#include "logging.h"


/*
GameMap* GameMap::getInstance()
{
    return NULL;
}
*/

class Pt {
public:
    Pt(int x0, int y0, int x1, int y1) : x(x0), y(y0), g(0.0f), parentX(x0), parentY(y0) {
        CalcH(x1, y1);
    }
    Pt(int x0, int y0) : x(x0), y(y0), g(0.0f), h(0.0f), parentX(x0), parentY(y0) {}

    bool operator==(const Pt& o) {
        return (x == o.x) && (y == o.y);
    }
    Pt& operator=(const Pt& o) {
        x = o.x;
        y = o.y;
        g = o.g;
        h = o.h;
        return *this;
    }

    void CalcH(int x1, int y1) {
        if (x == x1)
            h = fabs(y1 - y);
        else if (y == y1)
            h = fabs(x1 - x);
        else
            h = sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
    }
    int x, y;
    float g, h;
    int parentX, parentY;
};

GameMap::Chunk* GameMap::Chunk::upper()
{
    ChunkId dd(id);
    dd.y++;
    return mGameMap->findChunk(dd);
}

GameMap::Chunk* GameMap::Chunk::under()
{
    ChunkId dd(id);
    dd.y--;
    return mGameMap->findChunk(dd);
}

GameMap::Chunk* GameMap::Chunk::left()
{
    ChunkId dd(id);
    dd.x--;
    return mGameMap->findChunk(dd);
}

GameMap::Chunk* GameMap::Chunk::right()
{
    ChunkId dd(id);
    dd.x++;
    return mGameMap->findChunk(dd);
}

bool GameMap::Chunk::hasConnectedNeighbours()
{
    Chunk* c = upper();
    if (c && c->flag)
        return true;
    c = under();
    if (c && c->flag)
        return true;
    c = left();
    if (c && c->flag)
        return true;
    c = right();
    if (c && c->flag)
        return true;
    return false;
}


GameMap::Chunk::Chunk(GameMap::ChunkId _id, GameMap* gm)
    : id(_id)
    , flag(0)
    , mGameMap(gm)
{
#if 1
    int x = CHUNKWIDTH * id.x;
    int y = CHUNKHEIGHT * id.y;

    // normal
    for (int j=0; j<CHUNKHEIGHT; j++) {
        for (int i=0; i<CHUNKWIDTH; i++) {
            nodes[j][i].x = x + i;
            nodes[j][i].y = y + j;
            Node* n = &nodes[j][i];
            BIOME b = gm->mGenerator->getBiome(x + i, y + j);
            if (b == BIOME_FOREST) {
                n->type = NODE_TREE;
                n->blocksight = true;
                n->canpass = false;
            } else if (b == BIOME_PLANE) {
                n->type = NODE_GRASS;
                n->blocksight = false;
                n->canpass = true;
            } else if (b == BIOME_DESERT) {
                n->type = NODE_DIRT;
                n->blocksight = false;
                n->canpass = false;
            } else if (b == BIOME_OCEAN) {
                n->type = NODE_WATER;
                n->blocksight = false;
                n->canpass = false;
            }
        }
    }
#else
    // {{{
    GameMap::Node* n;
    int j, i;
    int d = sadv::dice(20, 1);
    if (d <= 10) {
        int w = 5 + sadv::dice(CHUNKWIDTH - 7, 1);
        int h = 5 + sadv::dice(CHUNKHEIGHT - 7, 1);
        int x = sadv::dice(CHUNKWIDTH - w, 1);
        int y = sadv::dice(CHUNKHEIGHT - h, 1);

        for (j=0; j<CHUNKHEIGHT; j++) {
            for (i=0; i<CHUNKWIDTH; i++) {
                nodes[j][i].x = x + i;
                nodes[j][i].y = y + j;
                n = &nodes[j][i];
                n->type = NODE_GRASS;
                n->blocksight = false;
                n->canpass = true;
            } 
        }
        for (j=y; j<y + h; j++) {
            for (i=x; i<x + w; i++) {
                n = &nodes[j][i];
                if (i == x || j == y || i == (x + w - 1) || j == (y + h - 1)) {
                    n->type = NODE_TREE;
                    n->blocksight = false;
                    n->canpass = false;
                } else {
                    n->type = NODE_DIRT;
                    n->blocksight = false;
                    n->canpass = true;
                }
            }
        }
        int di = sadv::dice(4, 1);
        int dp = -1;
        if (di < 3) {
            dp = x + sadv::dice(w - 2, 1);
            if (di == 1) {
                n = &nodes[y][dp];
            } else {
                n = &nodes[y + h - 1][dp];
            }
        } else {
            dp = y + sadv::dice(h - 2, 1);
            if (di == 3) {
                n = &nodes[dp][x];
            } else {
                n = &nodes[dp][x + w - 1];
            }
        }
        n->type = NODE_DIRT;
        n->canpass = true;
    } else {
        int x = CHUNKWIDTH * id.x;
        int y = CHUNKHEIGHT * id.y;

        // normal
        for (j=0; j<CHUNKHEIGHT; j++) {
            for (i=0; i<CHUNKWIDTH; i++) {
                nodes[j][i].x = x + i;
                nodes[j][i].y = y + j;
                n = &nodes[j][i];

                int d = sadv::dice(100, 1);
                if (d < 70) {
                    n->type = NODE_GRASS;
                    n->blocksight = false;
                    n->canpass = true;
                } else if (d < 80) {
                    n->type = NODE_DIRT;
                    n->blocksight = false;
                    n->canpass = false;
                } else if (d < 90) {
                    n->type = NODE_TREE;
                    n->blocksight = true;
                    n->canpass = false;
                } else {
                    n->type = NODE_WATER;
                    n->blocksight = false;
                    n->canpass = false;
                }
            }
        }
    }
    // }}}
#endif
}

GameMap::Node* GameMap::at(int x, int y)
{
    ChunkId id;
    id.fromPosition(x, y);

    Chunk* c = findChunk(id);
    if (!c)
        c = genChunk(id);

    int dx = x - id.getChunkStartX();
    int dy = y - id.getChunkStartY();
    Node* n = &(c->nodes[dy][dx]);
    return n;
}

GameMap::Chunk* GameMap::findChunk(ChunkId id)
{
    std::map<ChunkId, GameMap::Chunk*>::iterator it;
    it = chunks.find(id);
    if (it == chunks.end())
        return NULL;
    return it->second;
}

GameMap::Chunk* GameMap::genChunk(GameMap::ChunkId id)
{
    if (isChunkGenerated(id))
        return findChunk(id);

    Chunk* c = new Chunk(id, this);
    if (c) {
        chunks[id] = c;
    }
    return c;
}

void GameMap::genChunks()
{
    /*
    const int GENATONCE = 3;
    // const int GENRANGE = 5;
    ChunkId id = getCurrentChunkId();
    int i, j;
    int x = id.x;
    int y = id.y;

    for (i = x - GENATONCE; i < x + GENATONCE; i++) {
        for (j = y - GENATONCE; j < y + GENATONCE; j++) {
            id.set(i, j);
            if (!isChunkGenerated(id)) {
                genChunk(id);
            }
        }
    }
    */
    const int GENRANGE = 4 * CHUNKWIDTH;
    if (abs(centerX - lastX) > GENRANGE || abs(centerY - lastY) > GENRANGE || !isFirstRoomGenerated) {
        mGenerator->GenMapsNearChunk(getCurrentChunkId());
        lastX = centerX;
        lastY = centerY;
        isFirstRoomGenerated = true;
    }
}

GameMap::ChunkId GameMap::getCurrentChunkId()
{
    ChunkId id;
    id.fromPosition(centerX, centerY);
    return id;
}

GameMap::GameMap()
    : mGenerator(NULL)
    , centerX(0)
    , centerY(0)
    , lastX(0)
    , lastY(0)
    , isFirstRoomGenerated(false)
{
    mGenerator = new MapGeneratorRoom(this);
}

GameMap::~GameMap()
{
    if (mGenerator)
        delete mGenerator;

    std::map<ChunkId, Chunk*>::iterator it;
    for (it = chunks.begin(); it != chunks.end(); it++) {
        delete it->second;
    }
    chunks.clear();
}

bool GameMap::isChunkGenerated(int x, int y)
{
    ChunkId id(x, y);
    return isChunkGenerated(id);
}

bool GameMap::isChunkGenerated(ChunkId id)
{
    return (chunks.find(id) != chunks.end());
}

void GameMap::centerMap(int x, int y)
{
    centerX = x;
    centerY = y;
    genChunks();
}


bool GameMap::isNodeCanBeSeen(Node* n, Node* from)
{
    int dx = n->x - from->x;
    int dy = n->y - from->y;
    int ax = abs(dx);
    int ay = abs(dy);

    if (dx == 0 && dy == 0)
        return true;
    if (dx == 0) {
        int step = dy / abs(dy);
        for (int y = from->y + step; y != n->y; y += step) {
            Node* nd = at(n->x, y);
            if (nd->blocksight)
                return false;
        }
        return true;
    }

    if (dy == 0) {
        int step = dx / abs(dx);
        for (int x = from->x + step; x != n->x; x += step) {
            Node* nd = at(x, n->y);
            if (nd->blocksight)
                return false;
        }
        return true;
    }

    int sx = dx / abs(dx);
    int sy = dy / abs(dy);
    if (ax >= ay) {
        int x, y;
        for (x = from->x; x != n->x; x+= sx) {
            y = from->y + (x - from->x) * dy / dx;
            Node* nd = at(x, y);
            if (nd->blocksight)
                return false;
        }
    } else {
        int x, y;
        for (y = from->y; y != n->y; y+= sy) {
            x = from->x + (y - from->y) * dx / dy;
            Node* nd = at(x, y);
            if (nd->blocksight)
                return false;
        }
    }
    return true;
}


float GameMap::calcDistance(Node* n1, Node* n2)
{
    return calcDistance(n1->x, n1->y, n2->x, n2->y);
}

float GameMap::calcDistance(int x0, int y0, int x1, int y1)
{
    return sqrtf((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0  - y1));
}


bool GameMap::findPathTo(int x0, int y0, int x1, int y1, FpCallbackFunctor& fp)
{/*{{{*/
    std::list<Pt> OpenTable;
    std::list<Pt> ClosedTable;

    Node* n = at(x1, y1);
    if (!n->canpass)
        return false;

    Pt  pt(x0, y0, x1, y1);
    OpenTable.push_back(pt);

    int index = 0;
    while (!OpenTable.empty()) {
        index++;
        Pt pt = OpenTable.front();
        if (pt.x == x1 && pt.y == y1) {
            ClosedTable.push_back(pt);
            break;
        }

        OpenTable.pop_front();

        if (index > 500)
            return false;

        int x, y;
        for (x = pt.x - 1; x <= pt.x + 1; x++) {
            for (y = pt.y - 1; y <= pt.y + 1; y++) {
                if (x == pt.x && y == pt.y)
                    continue;

                // 是否走斜线
                if (x != pt.x && y != pt.y)
                    continue;

                Node* n = at(x, y);
                if (!n->canpass) {
                    continue;
                }
                if (n->creature != NULL) {
                    if (!(x == x1 && y == y1)) {
                        continue;
                    }
                }

                // 是否走到阴影里
                /*
                Node* st = at(x0, y0);
                if (!isNodeCanBeSeen(n, st))
                    continue;
                */


                Pt p0(x, y, x1, y1);
                if (std::find(ClosedTable.begin(), ClosedTable.end(), p0) != ClosedTable.end())
                    continue;
                if (pt.x == x || pt.y == y) {
                    p0.g = 1.0f;
                } else {
                    p0.g = 1.4f;
                }
                p0.parentX = pt.x;
                p0.parentY = pt.y;
                std::list<Pt>::iterator it = std::find(OpenTable.begin(), OpenTable.end(), p0);
                if (it != OpenTable.end()) {
                    if (p0.g + p0.h < (*it).g + (*it).h) {
                        (*it) = p0;
                    }
                } else {
                    // insert
                    if (OpenTable.size() == 0) {
                        OpenTable.push_back(p0);
                    } else {
                        std::list<Pt>::iterator it = OpenTable.begin();
                        it++;
                        while (it != OpenTable.end() && ((*it).g + (*it).h < p0.g + p0.h)) {
                            it++;
                        }
                        OpenTable.insert(it, p0);
                    }
                }
            }
        }
        ClosedTable.push_back(pt);
    }

    if (OpenTable.empty())
        return false;

    std::list<Pt> result;

    Pt last = Pt(x1, y1);
    // result.push_front(last);

    while (last.x != x0 || last.y != y0) {
        std::list<Pt>::iterator it = std::find(ClosedTable.begin(), ClosedTable.end(), last);
        if (it == ClosedTable.end()) {
            return false;
        }
        Pt next((*it).parentX, (*it).parentY);
        result.push_front(next);
        last = next;
    }

    for (std::list<Pt>::iterator it = result.begin(); it != result.end(); it++) {
        fp((*it).x, (*it).y);
    }
    return true;
}/*}}}*/

bool GameMap::findPath(int x0, int y0, int x1, int y1, FpCallbackFunctor& fp)
{
    Node* n = at(x1, y1);
    if (n->creature != NULL)
        return false;
    if (findPathTo(x0, y0, x1, y1, fp)) {
        fp(x1, y1);
        return true;
    }
    return false;
}



