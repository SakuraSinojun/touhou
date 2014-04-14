
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

    float dist = gml->gamemap().calcDistance(creature()->x, creature()->y, hero->x, hero->y);
    if (dist <= creature()->attackRange()) {
        if (!gml->currentCreatureAttack(hero))
            gml->idle();
    } else {
        if (creature()->speed() > 0) {
            if (!gml->moveCurrentCreature(pt))
                gml->idle();
        } else {
            gml->idle();
        }
    }
}

} // namespace ai
