
#include <stdio.h>
#include "GameMapLayer.h"
#include "HeroCreator.h"
#include "MapTile.h"
#include "GameScene.h"
#include "creature.h"
#include "gamemap.h"
#include "hero.h"
#include "logging.h"

USING_NS_CC;

#define MAPMOVETIMEPERGRID 0.3f

class CHelper {
public:
    void operator() (int x, int y) {
        CCPoint pt(x, y);
        nodes.push_back(pt);
    }
    std::list<CCPoint> nodes;
};


bool GameMapLayer::init()
{/*{{{*/
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

    mGrid = CCSprite::create("RedGrid.png");
    mGrid->setPosition(ccp(0, 0));
    mGrid->setVisible(false);
    this->addChild(mGrid, 11);
 
    mGridPosition.x = -100;
    mGridPosition.y = -100;

    // this->setTouchEnabled(true);
    // CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);


    refreshMap();
    return true;
}/*}}}*/

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
}

void GameMapLayer::onClick(cocos2d::CCPoint point)
{
    CCPoint pt = pointToMap(point);
    if (pt.x == mGridPosition.x && pt.y == mGridPosition.y) {
        onEnsureMove();
        return;
    }
    mGridPosition = pointToMap(point);

    removeAllPathGrids();
    Hero* hero = Hero::getInstance();
    CHelper ch;
    if (findPath(ccp(hero->x, hero->y), ccp(mGridPosition.x, mGridPosition.y), ch)) {
        mGrid->setVisible(false);
        std::list<CCPoint>::iterator it;
        // RUN_HERE() << "count = " << ch.nodes.size();
        for (it = ch.nodes.begin(); it != ch.nodes.end(); it++) {
            if (it == ch.nodes.begin())
                continue;

            CCSprite* sp = CCSprite::create("GreenGrid.png");
            PathGrid pg(sp, *it);
            mPathGrids.push_back(pg);
            this->addChild(sp, 12);
        }
    } else {
        mGrid->setVisible(true);
    }

    refreshMap();
}

void GameMapLayer::onEnsureMove()
{
    Hero* hero = Hero::getInstance();
    CCPoint last(hero->x, hero->y);
    std::list<PathGrid>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        CCPoint dp = ccpSub((*it).pt, last);
        last = (*it).pt;
        Direction d(dp.x, dp.y);
        mDirections.push_back(d);
    }

    GameScene* scene = (GameScene*)this->getParent();
    scene->setTouchEnabled(false);

    onMoveFinished(this);
}

void GameMapLayer::Walk(Direction direction)
{
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("hero.png");
    float w = texture->getContentSize().width / 4.0f;
    float h = texture->getContentSize().height / 4.0f;
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 2; i++) {
        animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
    }
    animation->setDelayPerUnit(MAPMOVETIMEPERGRID / 2.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    Hero::getInstance()->getSprite()->runAction(CCRepeatForever::create(animate));

    Hero* hero = Hero::getInstance();
    hero->move(direction.x(), direction.y(), &mGameMap);

    int i, j;
    for (i=0; i<MAPWIDTH; i++) {
        for (j=0; j<MAPHEIGHT; j++) {
            CCPoint pt = tiles[i][j]->getPosition();
            pt.x -= direction.x() * 32;
            pt.y -= direction.y() * 32;
            CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
            if (i == MAPWIDTH - 1 && j == MAPHEIGHT - 1) {
                CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameMapLayer::onMoveFinished));
                tiles[i][j]->runAction(CCSequence::create(mt, cf, NULL));
            } else {
                tiles[i][j]->runAction(mt);
            }
        }
    }

    std::list<PathGrid>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        PathGrid& pg = *it;
        if (pg.sprite) {
            CCPoint pt = pg.sprite->getPosition();
            pt.x -= direction.x() * 32;
            pt.y -= direction.y() * 32;
            CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
            pg.sprite->runAction(mt);
        }
    }

}

void GameMapLayer::onMoveFinished(cocos2d::CCObject* pSender)
{
    // RUN_HERE();
    Hero::getInstance()->getSprite()->stopAllActions();

    if (!mPathGrids.empty()) {
        PathGrid& pg = mPathGrids.front();
        this->removeChild(pg.sprite);
        mPathGrids.pop_front();
    }

    Hero* hero = Hero::getInstance();
    centerMap(ccp(hero->x, hero->y));

    // go on.
    if (mDirections.empty()) {
        GameScene* scene = (GameScene*)this->getParent();
        scene->setTouchEnabled(true);
    } else {
        Direction d = mDirections.front();
        mDirections.pop_front();
        Walk(d);
    }
}

void GameMapLayer::removeAllPathGrids()
{
    std::list<PathGrid>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        PathGrid& pg = *it;
        if (pg.sprite) {
            pg.sprite->setVisible(false);
            this->removeChild(pg.sprite);
        }
    }
    mPathGrids.clear();
}

#if 0
cocos2d::CCRect GameScene::gridToRect(cocos2d::CCPoint grid)
{
    CCRect  rect;
    rect.origin = ccp(grid.x * 32, grid.y * 32);
    rect.size = CCSizeMake(32, 32);
    return rect;
}

cocos2d::CCPoint GameScene::gridToPoint(cocos2d::CCPoint grid)
{
    CCPoint pt;
    pt.x = grid.x * 32 + 16;
    pt.y = grid.y * 32 + 16;
    return pt;
}

cocos2d::CCPoint GameScene::gridToMap(cocos2d::CCPoint grid)
{
    // mx = gx - x0 + hx
    int heroX = (MAPWIDTH - 2) / 2;
    int heroY = (MAPHEIGHT - 2) / 2;
    CCPoint pt = ccpSub(grid, ccp(heroX, heroY));
    pt = ccpAdd(pt, ccp(mHero.x, mHero.y));
    return pt;
}

cocos2d::CCPoint GameScene::mapToGrid(cocos2d::CCPoint pMap)
{
    // gx = mx - hx + x0
    int heroX = (MAPWIDTH - 2) / 2;
    int heroY = (MAPHEIGHT - 2) / 2;
    CCPoint pt = ccpSub(pMap, ccp(mHero.x, mHero.y));
    pt = ccpAdd(pt, ccp(heroX, heroY));
    return pt;
}
#endif
cocos2d::CCPoint GameMapLayer::pointToMap(cocos2d::CCPoint point)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCPoint center;
    center.x = origin.x + visibleSize.width / 2;
    center.y = origin.y + visibleSize.height / 2;

    CCPoint dd;
    dd.x = (MAPWIDTH % 2) / 2.0f;
    dd.y = (MAPHEIGHT % 2) / 2.0f;

    CCPoint dp;
    dp.x = floor((point.x - center.x) / 32.0f + dd.x);
    dp.y = floor((point.y - center.y) / 32.0f + dd.y);


    CCPoint pt;
    pt.x = mGameMap.centerX + dp.x;
    pt.y = mGameMap.centerY + dp.y;

    return pt;
}

cocos2d::CCRect GameMapLayer::gridToRect(cocos2d::CCPoint grid)
{
    return CCRectMake(0, 0, 0, 0);
}

cocos2d::CCPoint GameMapLayer::gridToPoint(cocos2d::CCPoint grid)
{
    return ccp(0, 0);
}

cocos2d::CCPoint GameMapLayer::gridToMap(cocos2d::CCPoint grid)
{
    return ccp(0, 0);
}

cocos2d::CCPoint GameMapLayer::mapToGrid(cocos2d::CCPoint pMap)
{
    return ccp(0, 0);
}

void GameMapLayer::refreshMap()
{/*{{{*/
    int i, j;
    int sx = mGameMap.centerX - MAPWIDTH / 2;
    int sy = mGameMap.centerY - MAPHEIGHT / 2;

    // this->removeAllChildren();

    CCObject* node = NULL;
    CCARRAY_FOREACH(this->getChildren(), node)
    {
        CCNode* n = (CCNode*)node;
        if (n->getTag() > 0) {
            n->setVisible(false);
        }
    }

    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    for (i=0; i<MAPWIDTH; i++) {
        for (j=0; j<MAPHEIGHT; j++) {
            int x = sx + i;
            int y = sy + j;
            GameMap::Node* n = mGameMap.at(x, y);

            if (!tiles[i][j]) {
                tiles[i][j] = MapTile::create();
                this->addChild(tiles[i][j], 0);
            }
            tiles[i][j]->setPosition(ccp(origin.x + (i - 1) * 32 + 16, origin.y + (j - 1) * 32 + 16));
            tiles[i][j]->setType(n->type);
            tiles[i][j]->setVisible(true);
            if (n->creature) {
                CCSprite * s = n->creature->getSprite();
                if (s) {
                    s->setPosition(ccp(origin.x + (i - 1) * 32 + 16, origin.y + (j - 1) * 32 + 16));
                    if (this->getChildByTag(s->getTag()) == NULL) {
                        this->addChild(s, 10); // , s->getTag());
                    }
                    s->setVisible(true);
                }
            }
        }
    }

    if (mPathGrids.empty()) {
        if (mGridPosition.x > sx && mGridPosition.x < sx + MAPWIDTH && mGridPosition.y > sy && mGridPosition.y < sy + MAPHEIGHT) {
            int x = mGridPosition.x - sx;
            int y = mGridPosition.y - sy;
            CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
            mGrid->setPosition(ccp(origin.x + (x - 1) * 32 + 16, origin.y + (y - 1) * 32 + 16));
        }
    } else {
        std::list<PathGrid>::iterator it;
        for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
            PathGrid& pg = *it;
            if (pg.sprite) {
                int x = pg.pt.x - sx;
                int y = pg.pt.y - sy;

                pg.sprite->setVisible(true);
                pg.sprite->setPosition(ccp(origin.x + (x - 1) * 32 + 16, origin.y + (y - 1) * 32 + 16));
            }
        }
    }

}/*}}}*/


