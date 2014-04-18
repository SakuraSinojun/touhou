

#include "item.h"

#include "GameResource.h"


USING_NS_CC;

Item::Item()
    : mSprite(NULL)
{
}

Item::~Item()
{
    if (mSprite)
        delete mSprite;
}

void Item::assignTag()
{
    mSprite->setTag(GameResource::nextTag());
}

CCSprite* Item::Sprite()
{
    return mSprite;
}

CCSprite* Item::createSprite()
{
    mSprite = new CCSprite();
    assignTag();
    return mSprite;
}

CCSprite* Item::createSprite(const char* file)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file);
    assignTag();
    return mSprite;
}

CCSprite* Item::createSprite(const char * file, cocos2d::CCRect& rect)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file, rect);
    assignTag();
    return mSprite;
}














