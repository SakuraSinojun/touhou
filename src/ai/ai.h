
#pragma once

#include <stdlib.h>
#include "GameMapLayer.h"

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

    virtual void turn(GameMapLayer* gml) = 0;

private:
    Creature* mCreature;
};

} // namespace ai;

