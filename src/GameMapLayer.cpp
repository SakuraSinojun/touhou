
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

#define MAPMOVETIMEPERGRID 0.15f


bool TileMapWrapper::init()
{/*{{{*/
    if (!CCLayer::init())
        return false;
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

    // refreshMap();

    return true;
}/*}}}*/

void TileMapWrapper::removeAllPathGrids()
{/*{{{*/
    std::list<PathGrid>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        PathGrid& pg = *it;
        if (pg.sprite) {
            pg.sprite->setVisible(false);
            this->removeChild(pg.sprite);
        }
    }
    mPathGrids.clear();
}/*}}}*/


void TileMapWrapper::refreshMap()
{/*{{{*/
    GameMapLayer* l = GameMapLayer::getInstance();
    int i, j;
    int sx = l->mGameMap.centerX - MAPWIDTH / 2;
    int sy = l->mGameMap.centerY - MAPHEIGHT / 2;

    int nx = MAPWIDTH - (DESIGNWIDTH / 32);
    int ny = MAPHEIGHT - (DESIGNHEIGHT / 32);
    nx /= 2;
    ny /= 2;

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

    Hero* hero = Hero::getInstance();
    GameMap::Node* heronode = l->mGameMap.at(hero->x, hero->y);

    for (i=0; i<MAPWIDTH; i++) {
        for (j=0; j<MAPHEIGHT; j++) {
            int x = sx + i;
            int y = sy + j;
            GameMap::Node* n = l->mGameMap.at(x, y);

            if (!tiles[i][j]) {
                tiles[i][j] = MapTile::create();
                this->addChild(tiles[i][j], 0);
            }
            tiles[i][j]->setPosition(ccp(origin.x + (i - nx) * 32 + 16, origin.y + (j - nx) * 32 + 16));
            tiles[i][j]->setType(n->type);
            if (l->mGameMap.isNodeCanBeSeen(n, heronode)) {
                tiles[i][j]->setVisible(true);
            } else {
                tiles[i][j]->setVisible(false);
            }
            if (n->creature) {
                CCSprite * s = n->creature->getSprite();
                if (s) {
                    s->setPosition(ccp(origin.x + (i - ny) * 32 + 16, origin.y + (j - ny) * 32 + 16));
                    if (this->getChildByTag(s->getTag()) == NULL) {
                        if (s->getTag() == 1000)
                            this->addChild(s, 11);
                        else
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
            mGrid->setPosition(ccp(origin.x + (x - nx) * 32 + 16, origin.y + (y - ny) * 32 + 16));
        }
    } else {
        std::list<PathGrid>::iterator it;
        for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
            PathGrid& pg = *it;
            if (pg.sprite) {
                int x = pg.pt.x - sx;
                int y = pg.pt.y - sy;

                pg.sprite->setVisible(true);
                pg.sprite->setPosition(ccp(origin.x + (x - nx) * 32 + 16, origin.y + (y - ny) * 32 + 16));
            }
        }
    }
}/*}}}*/

class CHelper {
public:
    void operator() (int x, int y) {
        CCPoint pt(x, y);
        nodes.push_back(pt);
    }
    std::list<CCPoint> nodes;
};


static GameMapLayer* g_gamemaplayer = NULL;

GameMapLayer* GameMapLayer::getInstance()
{
    return g_gamemaplayer;
}

bool GameMapLayer::init()
{/*{{{*/
    if (!CCLayer::init()) {
        return false;
    }
    g_gamemaplayer = this;

    tmw = TileMapWrapper::create();
    this->addChild(tmw);
    return true;
}/*}}}*/

void GameMapLayer::centerMap(cocos2d::CCPoint point)
{
    mGameMap.centerMap(point.x, point.y);
    tmw->refreshMap();
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
{/*{{{*/
    Hero* hero = Hero::getInstance();

    CCPoint pt = pointToMap(point);
    if (pt.x == tmw->mGridPosition.x && pt.y == tmw->mGridPosition.y) {
        if (pt.x != hero->x || pt.y != hero->y)
            onEnsureMove();
        return;
    }
    tmw->mGridPosition = pointToMap(point);

    tmw->removeAllPathGrids();
    CHelper ch;
    if (findPath(ccp(hero->x, hero->y), ccp(tmw->mGridPosition.x, tmw->mGridPosition.y), ch)) {
        tmw->mGrid->setVisible(false);
        std::list<CCPoint>::iterator it;
        // RUN_HERE() << "count = " << ch.nodes.size();
        for (it = ch.nodes.begin(); it != ch.nodes.end(); it++) {
            if (it == ch.nodes.begin())
                continue;

            CCSprite* sp = CCSprite::create("GreenGrid.png");
            TileMapWrapper::PathGrid pg(sp, *it);
            tmw->mPathGrids.push_back(pg);
            tmw->addChild(sp, 12);
        }
    } else {
        tmw->mGrid->setVisible(true);
    }

    tmw->refreshMap();
}/*}}}*/

void GameMapLayer::onEnsureMove()
{/*{{{*/
    Hero* hero = Hero::getInstance();
    CCPoint last(hero->x, hero->y);
    std::list<TileMapWrapper::PathGrid>::iterator it;
    for (it = tmw->mPathGrids.begin(); it != tmw->mPathGrids.end(); it++) {
        CCPoint dp = ccpSub((*it).pt, last);
        last = (*it).pt;
        Direction d(dp.x, dp.y);
        mDirections.push_back(d);
    }

    GameScene* scene = (GameScene*)this->getParent();
    scene->setTouchEnabled(false);

    onMoveFinished(this);
}/*}}}*/

void GameMapLayer::Walk(Direction direction)
{
    Hero* hero = Hero::getInstance();

    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("hero.png");
    float w = texture->getContentSize().width / 4.0f;
    float h = texture->getContentSize().height / 4.0f;
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 4; i++) {
        animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
    }
    animation->setDelayPerUnit(MAPMOVETIMEPERGRID / 2.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    hero->getSprite()->runAction(CCRepeatForever::create(animate));

    CCPoint pt = hero->getSprite()->getPosition();
    pt.x += direction.x() * 32;
    pt.y += direction.y() * 32;
    CCMoveTo* hmt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    hero->getSprite()->runAction(hmt);


    hero->move(direction.x(), direction.y(), &mGameMap);

    pt = tmw->getPosition();
    pt.x -= direction.x() * 32;
    pt.y -= direction.y() * 32;
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameMapLayer::onMoveFinished));
    tmw->runAction(CCSequence::create(mt, cf, NULL));

    /*
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
    */

}

void GameMapLayer::onMoveFinished(cocos2d::CCObject* pSender)
{
    // RUN_HERE();
    Hero::getInstance()->getSprite()->stopAllActions();

    if (!tmw->mPathGrids.empty()) {
        TileMapWrapper::PathGrid& pg = tmw->mPathGrids.front();
        tmw->removeChild(pg.sprite);
        tmw->mPathGrids.pop_front();
    }

    tmw->setPosition(ccp(0, 0));

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


