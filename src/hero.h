
#pragma once

#include "creature.h"
class Hero : public Creature
{
public:
    Hero() : x(0), y(0) {}

    void MoveTo(int x_, int y_) { x = x_, y = y_; }
    bool move(int dx, int dy) {
        x += dx;
        y += dy;
        return true;
    }

    int x;
    int y;
};

