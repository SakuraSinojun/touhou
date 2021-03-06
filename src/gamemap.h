
#pragma once


#include <map>
#include <list>
#include <stdlib.h>
#include "cocos2d.h"
#include "ornaments/ornament.h"
#include "items/item.h"

#define CHUNKWIDTH  16
#define CHUNKHEIGHT CHUNKWIDTH


class MapGenerator;
class Creature;
class GameMap
{
public:
    typedef enum {
        NODE_NONE = 0,
        NODE_GRASS,
        NODE_DIRT,
        NODE_TREE,
        NODE_WATER,
        NODE_MAX,
    } NODETYPE;

    typedef enum {
        BIOME_FOREST,
        BIOME_PLANE,
        BIOME_DESERT,
        BIOME_RIVER,
        BIOME_OCEAN,
    } BIOME;

    class Node {
    public:
        Node()
            : type(NODE_GRASS)
            , x(0)
            , y(0)
            , creature(NULL)
            , isWall(true)
            , hasConsidered(false)
            , biome(BIOME_FOREST)
            , explored(false)
            , ornament(NULL)
            , blocksight(false)
            , canpass(true)
        {}
        ~Node() {}
        NODETYPE type;
        bool isBlockSight() {
            if (blocksight)
                return true;
            if (ornament != NULL)
                return ornament->blocksight;
            return false;
        }

        bool canPass() {
            if (!canpass)
                return false;
            if (ornament != NULL)
                return ornament->canpass;
            return true;
        }

        int x;
        int y;
        Creature* creature;
        bool isWall;
        bool hasConsidered;
        BIOME   biome;
        bool explored;
        Ornament* ornament;
        std::list<Item*>    items;
        bool blocksight;
        bool canpass;
    };

    class ChunkId {
    public:
        ChunkId() : x(0), y(0) {}
        ChunkId(int xx, int yy) : x(xx), y(yy) {}
        ~ChunkId() {}
        ChunkId(const ChunkId& o) {
            x = o.x;
            y = o.y;
        }
        ChunkId& operator=(const ChunkId& o) {
            x = o.x;
            y = o.y;
            return *this;
        }
        bool operator==(const ChunkId& o) {
            return (x == o.x) && (y == o.y);
        }
        bool operator<(const ChunkId& o) const {
            if (x == o.x)
                return (y < o.y);
            return (x < o.x);
        }
        void set(int xx, int yy) {
            x = xx;
            y = yy;
        }
        void fromPosition(int xx, int yy) {
            if (xx < 0) {
                xx -= (CHUNKWIDTH - 1);
            }
            if (yy < 0) {
                yy -= (CHUNKHEIGHT - 1);
            }
            x = xx / CHUNKWIDTH;
            y = yy / CHUNKHEIGHT;
        }
        int getChunkStartX() {
            return CHUNKWIDTH * x;
        }
        int getChunkStartY() {
            return CHUNKHEIGHT * y;
        }
        int getChunkCenterX() {
            return CHUNKWIDTH * x + CHUNKWIDTH / 2;
        }
        int getChunkCenterY() {
            return CHUNKHEIGHT * y + CHUNKHEIGHT / 2;
        }
        int x;
        int y;
    };

    class Chunk {
    public:
        Chunk(ChunkId _id, GameMap* gm);
        virtual ~Chunk();

        Chunk* upper();
        Chunk* under();
        Chunk* left();
        Chunk* right();
        bool hasConnectedNeighbours();

        ChunkId id;
        Node nodes[CHUNKHEIGHT][CHUNKWIDTH];
        int flag;
    private:
        void MakeRandomNode();
        GameMap* mGameMap;
    };

public:
    // static GameMap* getInstance();

    GameMap();
    ~GameMap();

    Node* at(int x, int y);
    Node* at(cocos2d::CCPoint pt) { return at(pt.x, pt.y); }
    void centerMap(int x, int y);

    Chunk* findChunk(ChunkId id);

    class FpCallbackFunctor {
    public:
        virtual void operator() (int x, int y) = 0;
    };

    // 包含(x1, y1)
    bool findPath(int x0, int y0, int x1, int y1, FpCallbackFunctor& fp);

    // 不包含(x1, y1)
    bool findPathTo(int x0, int y0, int x1, int y1, FpCallbackFunctor& fp);

    bool findPath1Step(int x0, int y0, int x1, int y1, FpCallbackFunctor& fp);

    bool isNodeCanBeSeen(Node* n, Node* from);
    bool isNodeCanBeSeen(int dx, int dy, int fx, int fy);
    float calcDistance(Node* n1, Node* n2);
    float calcDistance(int x0, int y0, int x1, int y1);

private:
    std::map<ChunkId, Chunk*>   chunks;

    Chunk* genChunk(ChunkId id);
    void genChunks();
    ChunkId getCurrentChunkId();

    bool isChunkGenerated(int x, int y);
    bool isChunkGenerated(ChunkId id);

    MapGenerator*  mGenerator;

public:
    int centerX, centerY;
    
    // for generator
    int lastX;
    int lastY;
    bool isFirstRoomGenerated;
};

class FpHelper : public GameMap::FpCallbackFunctor {
public:
    virtual void operator() (int x, int y) {
        cocos2d::CCPoint pt(x, y); 
        nodes.push_back(pt);
    }   
    std::list<cocos2d::CCPoint> nodes;
};




