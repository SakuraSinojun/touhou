
#pragma once

#include "ai.h"

namespace ai { 

class Normal : public AI
{
public:
    Normal() : AI() {}
    Normal(Creature* c) : AI(c) {}
    virtual ~Normal() {}

    virtual void turn(GameMapLayer* gml);
};

} // namespace ai;

