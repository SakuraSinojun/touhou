

#include "sword.h"
#include <stdlib.h>

USING_NS_CC;

Sword::Sword()
    : Item()
{
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("items/items1.png");
    
    int x = rand() % 15 + 1;
    int y = rand() % 8;
    // CCRect  rect(24 * 1, 0, 24, 24);
    CCRect  rect(x * 24, y * 24, 24, 24);
    createSprite("items/items1.png", rect);
    Sprite()->setTexture(texture);
    Sprite()->setTextureRect(rect);
    Sprite()->setScale(32.0f / 24.0f);
}

Sword::~Sword()
{
}

