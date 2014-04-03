

#include "hero.h"



static Hero* g_hero;

Hero* Hero::getInstance()
{
    if (!g_hero)
        g_hero = new Hero();
    return g_hero;
}

