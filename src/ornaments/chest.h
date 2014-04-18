
#pragma once

#include "ornament.h"

class Chest : public Ornament
{
public:
    Chest();
    virtual ~Chest();

    virtual bool active();
private:
    bool mOpened;
};

