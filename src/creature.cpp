
#include "creature.h"


USING_NS_CC;

static int g_tag = 1000;

Creature::Creature()
    : mSprite(NULL)
    , mCurrentHP(10)
{
}

Creature::~Creature()
{
    if (mSprite)
        delete mSprite;
}

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
    mSprite = new CCSprite();
    mSprite->setTag(g_tag++);
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char* file)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file);
    mSprite->setTag(g_tag++);
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char * file, cocos2d::CCRect& rect)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file, rect);
    mSprite->setTag(g_tag++);
    return mSprite;
}

int Creature::currentHp()
{
    return mCurrentHP;
}

int Creature::maxHp()
{
    return 10;
}



