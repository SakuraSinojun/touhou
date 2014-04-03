
#include "creature.h"


USING_NS_CC;

static int g_tag = 1000;

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

cocos2d::CCSprite* Creature::createSprite()
{
    mSprite = CCSprite::create();
    mSprite->setTag(g_tag++);
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char* file)
{
    mSprite = CCSprite::create(file);
    mSprite->setTag(g_tag++);
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char * file, cocos2d::CCRect& rect)
{
    mSprite = CCSprite::create(file, rect);
    mSprite->setTag(g_tag++);
    return mSprite;
}



