
#include "normal.h"
#include "hero.h"

#include "cocos2d.h"
USING_NS_CC;

namespace ai {

void Normal::turn(GameMapLayer* gml)
{
    if (!creature())
        return;
    
    Hero* hero = Hero::getInstance();
    CCPoint pt(hero->x, hero->y);
    if (gml->turnCount() % 3 == 1) {
        if (!gml->moveCurrentCreature(pt))
            gml->idle();
    } else {
        gml->idle();
    }
}

} // namespace ai
