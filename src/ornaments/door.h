
#pragma once

#include "ornament.h"

class Door : public Ornament
{
public:
    Door();
    virtual ~Door();

    virtual bool active();
private:
    bool mOpened;
};

