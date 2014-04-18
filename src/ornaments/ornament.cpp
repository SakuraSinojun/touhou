
#include "ornament.h"
#include "chest.h"
#include "GameResource.h"
#include <stdlib.h>

USING_NS_CC;

Ornament::Ornament()
    : canpass(false)
    , blocksight(true)
    , mSprite(NULL)
{
}

Ornament::~Ornament()
{
    if (mSprite)
        delete mSprite;
}

Ornament* Ornament::create()
{
    int d = rand() % 100;
    if (d == 0) {
        return new Chest();
    }
    return NULL;
}

CCSprite* Ornament::Sprite()
{
    return mSprite;
}

CCSprite* Ornament::createSprite()
{
    mSprite = new CCSprite();
    mSprite->setTag(GameResource::nextTag());
    return mSprite;

}

CCSprite* Ornament::createSprite(const char* file)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file);
    mSprite->setTag(GameResource::nextTag());
    return mSprite;
}

CCSprite* Ornament::createSprite(const char * file, cocos2d::CCRect& rect)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file, rect);
    mSprite->setTag(GameResource::nextTag());
    return mSprite;
}

bool Ornament::active()
{
    return false;
}




