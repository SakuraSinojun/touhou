
#include "chest.h"


USING_NS_CC;

Chest::Chest()
    : mOpened(false)
{
    canpass = false;
    blocksight = false;

    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("ornaments/chest.png");
    CCRect rect(0, 0, 32, 32);
    createSprite("ornaments/chest.png", rect);
    Sprite()->setTexture(texture);

}

Chest::~Chest()
{
}

bool Chest::active()
{
    if (!mOpened) {
        CCRect rect(0, 32 * 3, 32, 32);
        Sprite()->setTextureRect(rect);
        mOpened = true;
        return true;
    }
    return false;
}

