
#include "door.h"


USING_NS_CC;

Door::Door()
    : mOpened(false)
{
    canpass = false;
    blocksight = true;

    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ornaments/door3.png");
    CCRect rect(112, 0, 64, 64);
    createSprite("ornaments/chest.png", rect);
    Sprite()->setTexture(texture);
    Sprite()->setTextureRect(rect);
    Sprite()->setScale(0.5f);
}

Door::~Door()
{
}

bool Door::active()
{
    if (!mOpened) {
        CCRect rect(112, 64 * 3, 64, 64);
        Sprite()->setTextureRect(rect);
        Sprite()->setScale(0.5f);
        mOpened = true;
        canpass = true;
        blocksight = false;
    } else {
        CCRect rect(112, 0, 64, 64);
        Sprite()->setTextureRect(rect);
        Sprite()->setScale(0.5f);
        mOpened = false;
        canpass = false;
        blocksight = true;
    }
    return true;
}

