

#include "gamemap.h"

#include "dice.h"
#include <algorithm>
#include <list>
#include <math.h>
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


GameMap::Chunk::Chunk(GameMap::ChunkId _id)
    : id(_id)
{
    GameMap::Node* n;
    int j, i;
    int d = sadv::dice(20, 1);
    d = 11;
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
    Chunk* c = new Chunk(id);
    if (c) {
        chunks[id] = c;
    }
    return c;
}

void GameMap::genChunks()
{
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
}

GameMap::ChunkId GameMap::getCurrentChunkId()
{
    ChunkId id;
    id.fromPosition(centerX, centerY);
    return id;
}

GameMap::GameMap()
    : centerX(0)
    , centerY(0)
{
}

GameMap::~GameMap()
{
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

bool GameMap::findPath(int x0, int y0, int x1, int y1, FpCallbackFunctor& fp)
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
                        std::list<Pt>::iterator it2 = OpenTable.begin();
                        std::list<Pt>::iterator it = it2;
                        it2++;
                        while (it2 != OpenTable.end() && ((*it2).g + (*it2).h < p0.g + p0.h)) {
                            it = it2;
                            it2++;
                        }
                        OpenTable.insert(it2, p0);
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
    result.push_front(last);

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



