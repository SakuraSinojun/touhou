
#include "creature.h"


USING_NS_CC;

void Creature::Attach(CCSprite* cs)
{ 
    if (cs) {
        mSprite = cs;
    } 
}

CCSprite* Creature::getSprite()
{ 
    return mSprite; 
}

