
#include <stdio.h>
#include "GameMapLayer.h"
#include "HeroCreator.h"
#include "MapTile.h"
#include "gamemap.h"
#include "logging.h"

USING_NS_CC;

bool GameMapLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    // CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    // CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    int i, j;
    for (i=0; i<MAPWIDTH; i++) {
        for (j=0; j<MAPHEIGHT; j++) {
            tiles[i][j] = NULL;
        }
    }
    refreshMap();

    // this->setTouchEnabled(true);
    // CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);

    return true;
}

void GameMapLayer::centerMap(cocos2d::CCPoint point)
{
    mGameMap.centerMap(point.x, point.y);
    refreshMap();
}

cocos2d::CCPoint GameMapLayer::getMapCenter()
{
    return ccp(mGameMap.centerX, mGameMap.centerY);
}

void GameMapLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void GameMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    // CCTouch* touch = (CCTouch*)(touches->anyObject());
    // CCPoint pt = touch->getLocation();
    // RUN_HERE() << "touch: (" << pt.x << ", " << pt.y << ")";
}

void GameMapLayer::at(cocos2d::CCPoint point)
{
    mCurrentPoint = point;
}

void GameMapLayer::refreshMap()
{
    int i, j;
    int sx = mGameMap.centerX - MAPWIDTH / 2;
    int sy = mGameMap.centerY - MAPHEIGHT / 2;

    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    for (i=0; i<MAPWIDTH; i++) {
        for (j=0; j<MAPHEIGHT; j++) {
            int x = sx + i;
            int y = sy + j;
            GameMap::Node* n = mGameMap.at(x, y);

            if (!tiles[i][j]) {
                tiles[i][j] = MapTile::create();
                tiles[i][j]->setPosition(ccp(origin.x + (i - 1) * 32 + 16, origin.y + (j - 1) * 32 + 16));
                this->addChild(tiles[i][j], 0);
            }
            tiles[i][j]->setType(n->type);
        }
    }

}

