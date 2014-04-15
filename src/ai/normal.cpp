
#include "normal.h"
#include "hero.h"
#include "GameResource.h"
#include "MapLayer.h"
#include "cocos2d.h"
USING_NS_CC;

namespace ai {

void Normal::turn(MapLayer* gml)
{
    if (!creature())
        return;
    
    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();
    CCPoint pt(hero->x, hero->y);

    float dist = gamemap->calcDistance(creature()->x, creature()->y, hero->x, hero->y);
    if (dist <= creature()->attackRange()) {
        // if (!gml->currentCreatureAttack(hero))
        //    gml->idle();
    } else {
        creature()->findPathAndMove(hero->x, hero->y);
    }
}

} // namespace ai
