
#pragma once

#include <stdlib.h>

class Creature;

namespace ai {

class AI
{
public:
    AI() : mCreature(NULL) {}

    void attach(Creature* c) { mCreature = c; }
    Creature* creature() { return mCreature; }

    virtual void turn() = 0;

private:
    Creature* mCreature;
};

} // namespace ai;

