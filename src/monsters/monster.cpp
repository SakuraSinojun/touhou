
#include "monster.h"
#include "dark_ghost.h"
#include "red_wiz.h"

#include "dice.h"

Monster* Monster::create()
{
    int d = sadv::dice(6);
    if (d < 3) {
        return new DarkGhost();
    } else {
        return new RedWiz();
    }
}


