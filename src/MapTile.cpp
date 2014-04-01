

#include "cocos2d.h"
#include "MapTile.h"

USING_NS_CC;

bool MapTile::init()
{
    if (!CCSprite::init()) {
        return false;
    }
    return true;
}

void MapTile::setType(GameMap::NODETYPE type)
{
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("tilea2.png");
    // float w = texture->getContentSize().width  / 16;
    // float y = texture->getContentSize().width  / 12;
    this->setTexture(texture);
    if (type == GameMap::NODE_GRASS)
        this->setTextureRect(CCRectMake(0, 0, 32, 32));
    else if (type == GameMap::NODE_DIRT)
        this->setTextureRect(CCRectMake(0, 32 * 4, 32, 32));
    else if (type == GameMap::NODE_TREE)
        this->setTextureRect(CCRectMake(8 * 32, 0, 32, 32));
    else if (type == GameMap::NODE_WATER)
        this->setTextureRect(CCRectMake(14 * 32, 9 * 32, 32, 32));
    else 
        this->setTextureRect(CCRectMake(0, 0, 32, 32));
}

