
#pragma once

#include <stdlib.h>

class MapLayer;
class Creature;
namespace ai {

class AI
{
public:
    AI() : mCreature(NULL) {}
    AI(Creature* c) : mCreature(c) {}
    virtual ~AI() {}

    void attach(Creature* c) { mCreature = c; }
    Creature* creature() { return mCreature; }

    virtual void turn(MapLayer* gml) = 0;

private:
    Creature* mCreature;
};

} // namespace ai;

