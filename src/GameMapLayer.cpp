
#include <stdio.h>
#include <algorithm>
#include "GameMapLayer.h"
#include "GameOverScene.h"
#include "HeroCreator.h"
#include "MapTile.h"
#include "GameScene.h"
#include "creature.h"
#include "gamemap.h"
#include "hero.h"
#include "logging.h"

USING_NS_CC;


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
    mIsGreenGrid = false;

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
    // GameMapLayer* gml = (GameMapLayer*)getParent();

    GameMapLayer* l = GameMapLayer::getInstance();
    if (l == NULL)
        return;

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

            if (!n->explored) {
                if (l->mGameMap.isNodeCanBeSeen(n, heronode)) {
                    n->explored = true;
                }
            }
             
            if (n->explored) {
                tiles[i][j]->setVisible(true);
            } else {
                tiles[i][j]->setVisible(false);
            }
            if (n->creature) {
                CCSprite* s = n->creature->getSprite();
                CCSprite* b = n->creature->getBar();
                if (s) {
                    s->setPosition(ccp(origin.x + (i - nx) * 32 + 16, origin.y + (j - ny) * 32 + 16));
                    b->setPosition(ccp(origin.x + (i - nx) * 32 + 16, origin.y + (j - ny) * 32 + 16 + 32));

                    if (this->getChildByTag(s->getTag()) == NULL) {
                        if (s->getTag() == 1000) {
                            this->addChild(s, 11);
                            this->addChild(b, 11);
                        } else {
                            this->addChild(s, 10); // , s->getTag());
                            this->addChild(b, 10);
                        }
                    }
                    s->setVisible(tiles[i][j]->isVisible());
                    b->setVisible(tiles[i][j]->isVisible());
                    /*
                    if (gml->getClickType() == GameMapLayer::CT_ATTACK) {
                        b->setVisible(tiles[i][j]->isVisible());
                    } else {
                        b->setVisible(false);
                    }
                    */
                    // s->setVisible(true);
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
                // RUN_HERE() << pg.sprite->getPosition();
            }
        }
    }
}/*}}}*/

void TileMapWrapper::useRedGrid()
{/*{{{*/
    if (!isGreenGrid())
        return;

    this->removeChild(mGrid);
    mGrid = CCSprite::create("RedGrid.png");
    this->addChild(mGrid, 11);

    mIsGreenGrid = false;
}/*}}}*/

void TileMapWrapper::useGreenGrid()
{/*{{{*/
    if (isGreenGrid())
        return;
    this->removeChild(mGrid);
    mGrid = CCSprite::create("GreenGrid.png");
    this->addChild(mGrid, 11);
    mIsGreenGrid = true;
}/*}}}*/

void TileMapWrapper::showGrid(bool show)
{
    mGrid->setVisible(show);
    if (!show) {
        mGridPosition.x = Hero::getInstance()->x;
        mGridPosition.y = Hero::getInstance()->y;
    }
}


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
    mClickType = CT_MOVE;

    mCurrentCreature = Hero::getInstance();
    mCurrentTurn = 0;
    mAttackedCreature = NULL;

    return true;
}/*}}}*/

void GameMapLayer::centerCreature(Creature* c)
{
    centerMap(ccp(c->x, c->y));
}

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

void GameMapLayer::setClickType(CLICKTYPE type)
{
    Hero* hero = Hero::getInstance();
    if (mClickType != type) {
        tmw->removeAllPathGrids();
        tmw->mGridPosition.x = hero->x;
        tmw->mGridPosition.y = hero->y;
        tmw->useRedGrid();
        tmw->showGrid(false);
    }
    mClickType = type;
    tmw->refreshMap();
}


bool GameMapLayer::currentCreatureAttack(Creature* c)
{
    if (mCurrentCreature == NULL)
        return false;
    if (mAttackedCreature != NULL)
        return false;

    mAttackedCreature = c;

    // disable touch
    Hero::getInstance()->onEndTurn(this);
    /*
    GameScene* s = (GameScene*)this->getParent();
    s->setTouchEnabled(false);
    */

    mEmitter = CCParticleMeteor::create();
    this->addChild(mEmitter);
    mEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fireball.png"));
    mEmitter->setScale(0.5f);
    CCPoint pt = mapToPoint(ccp(mCurrentCreature->x, mCurrentCreature->y));
    mEmitter->setPosition(pt);

    pt = mapToPoint(ccp(c->x, c->y));
    CCMoveTo* ct = CCMoveTo::create(0.5f, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameMapLayer::onEmitterMoveFinished));
    mEmitter->runAction(CCSequence::create(ct, cf, NULL));

    return true;
}

void GameMapLayer::onClick(cocos2d::CCPoint point)
{/*{{{*/
    Hero* hero = Hero::getInstance();

    if (mClickType == CT_MOVE) {
        // {{{
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
            int speed = hero->speed();
            bool expandSpeed = false;
            if (speed <= 0) {
                speed = hero->maxSpeed();
                expandSpeed = true;
            }
            for (it = ch.nodes.begin(); it != ch.nodes.end(); it++) {
                if (it == ch.nodes.begin())
                    continue;

                CCSprite* sp;
                if (speed > 0) {
                    if (expandSpeed)
                        sp = CCSprite::create("YellowGrid.png");
                    else
                        sp = CCSprite::create("GreenGrid.png");
                } else {
                    sp = CCSprite::create("RedGrid.png");
                }
                speed--;

                TileMapWrapper::PathGrid pg(sp, *it);
                tmw->mPathGrids.push_back(pg);
                tmw->addChild(sp, 12);
            }
        } else {
            tmw->mGrid->setVisible(true);
        }

        tmw->refreshMap();
        // }}}
    } else if (mClickType == CT_ATTACK) {
        // {{{
        CCPoint pt = pointToMap(point);
        if (pt.x == tmw->mGridPosition.x && pt.y == tmw->mGridPosition.y) {
            if ((pt.x != hero->x || pt.y != hero->y) && tmw->isGreenGrid())
                onEnsureAttack();
            return;
        }

        tmw->mGridPosition = pointToMap(point);
        Creature* c = mGameMap.at(pt.x, pt.y)->creature;

        tmw->useGreenGrid();
        if (!c) {
            tmw->useRedGrid();
        }
        GameMap::Node* h = mGameMap.at(hero->x, hero->y);
        GameMap::Node* t = mGameMap.at(pt.x, pt.y);

        float dist = mGameMap.calcDistance(h, t);
        if (dist > hero->attackRange())
            tmw->useRedGrid();
        if (pt.x == hero->x && pt.y == hero->y) {
            tmw->showGrid(false);
        } else {
            tmw->showGrid(true);
        }

        tmw->refreshMap();
        // }}}
    } else if (mClickType == CT_EXAMINE) {
        CCPoint pt = pointToMap(point);
        tmw->mGridPosition = pt;
        tmw->useGreenGrid();
        onEnsureExamine();

        tmw->refreshMap();
    }
}/*}}}*/

void GameMapLayer::onEnsureAttack()
{/*{{{*/
    // RUN_HERE();
    // Hero* hero = Hero::getInstance();
    GameMap::Node* n = mGameMap.at(tmw->mGridPosition.x, tmw->mGridPosition.y);
    Creature* c = n->creature;
    if (c != NULL) {
        currentCreatureAttack(c);
        /*
        GameScene* s = (GameScene*)this->getParent();
        s->setTouchEnabled(false);

        mEmitter = CCParticleMeteor::create();
        this->addChild(mEmitter);
        mEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fireball.png"));
        mEmitter->setScale(0.5f);

        CCPoint pt = mapToPoint(ccp(hero->x, hero->y));
        mEmitter->setPosition(pt);

        pt = mapToPoint(tmw->mGridPosition);
        CCMoveTo* ct = CCMoveTo::create(0.5f, pt);
        CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameMapLayer::onEmitterMoveFinished));
        mEmitter->runAction(CCSequence::create(ct, cf, NULL));
        */
    }
}/*}}}*/

void GameMapLayer::onCreatureDie(Creature* c)
{
    if (c == NULL)
        return;

    if (c == Hero::getInstance()) {
        g_gamemaplayer = NULL;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, GameOverScene::create()));
        return;
    }

    mCurrentCreature->addExp(c->expOnDeath());

    removeActiveCreature(c);
    tmw->removeChild(c->getSprite());
    tmw->removeChild(c->getBar());

    GameMap::Node* n = mGameMap.at(c->x, c->y);
    n->creature = NULL;
    delete c;
}

void GameMapLayer::onEmitterMoveFinished(cocos2d::CCObject* pSender)
{/*{{{*/
    this->removeChild(mEmitter);

    if (mCurrentCreature == NULL)
        return;

    // mAttackedCreature;
    // Hero* hero = Hero::getInstance();
    // Creature* c = n->creature;
    if (mAttackedCreature != NULL) {
        mCurrentCreature->attack(mAttackedCreature);

        // die
        if (mAttackedCreature->currentHp() <= 0) {
            onCreatureDie(mAttackedCreature);
            tmw->showGrid(false);
        }
    }
    tmw->refreshMap();

    // GameScene* s = (GameScene*)this->getParent();
    // s->setTouchEnabled(true);

    mAttackedCreature = NULL;
    onTurn(0.5f);
}/*}}}*/

void GameMapLayer::onEnsureExamine()
{
    RUN_HERE();
}

void GameMapLayer::onEnsureMove()
{/*{{{*/
    if (Hero::getInstance()->speed() <= 0) {
        Hero::getInstance()->resetSpeed();
    }
    moveCurrentCreature(tmw->mGridPosition);
    return;
}/*}}}*/

void GameMapLayer::Walk(Direction direction)
{/*{{{*/
    if (mCurrentCreature == NULL)
        return;

    mCurrentCreature->StartWalkingAnimation(direction.x(), direction.y());
    CCPoint pt = mCurrentCreature->getSprite()->getPosition();
    pt.x += direction.x() * 32;
    pt.y += direction.y() * 32;
    CCMoveTo* hmt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mCurrentCreature->getSprite()->runAction(hmt);
    pt = mCurrentCreature->getBar()->getPosition();
    pt.x += direction.x() * 32;
    pt.y += direction.y() * 32;
    CCMoveTo* hmtb = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mCurrentCreature->getBar()->runAction(hmtb);


    mCurrentCreature->move(direction.x(), direction.y(), &mGameMap);
    mCurrentCreature->speed()--;

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

}/*}}}*/

void GameMapLayer::onMoveFinished(cocos2d::CCObject* pSender)
{/*{{{*/
    // RUN_HERE();
    if (mCurrentCreature == NULL)
        return;
    mCurrentCreature->StopWalkingAnimation();
    if (mCurrentCreature->speed() <= 0) {
        tmw->removeAllPathGrids();
        mDirections.clear();
        tmw->setPosition(ccp(0, 0));
        centerMap(ccp(mCurrentCreature->x, mCurrentCreature->y));

        onTurn(0.5f);
        return;
    }

    if (!tmw->mPathGrids.empty()) {
        TileMapWrapper::PathGrid& pg = tmw->mPathGrids.front();
        tmw->removeChild(pg.sprite);
        tmw->mPathGrids.pop_front();
    }
    tmw->setPosition(ccp(0, 0));
    centerMap(ccp(mCurrentCreature->x, mCurrentCreature->y));

    // go on.
    if (mDirections.empty()) {
        // GameScene* scene = (GameScene*)this->getParent();
        // scene->setTouchEnabled(true);
        
        // 就算没走到最大格数，这个回合也是移动完了的。
        mCurrentCreature->speed() = 0;
        tmw->setPosition(ccp(0, 0));
        centerMap(ccp(mCurrentCreature->x, mCurrentCreature->y));

        onTurn(0.5f);
    } else {
        Direction d = mDirections.front();
        mDirections.pop_front();
        Walk(d);
    }
}/*}}}*/

bool GameMapLayer::moveCurrentCreature(cocos2d::CCPoint dest)
{/*{{{*/
    if (mCurrentCreature == NULL)
        return false;

    Creature*& c = mCurrentCreature;
    // centerCreature(c);

    if (tmw->mPathGrids.empty()) {
        CHelper ch;
        if (!findPath(ccp(c->x, c->y), dest, ch, false))
            return false;

        // RUN_HERE() << "count = " << ch.nodes.size();
        int speed = c->speed();
        std::list<CCPoint>::iterator it;
        for (it = ch.nodes.begin(); it != ch.nodes.end(); it++) {
            if (it == ch.nodes.begin())
                continue;
            CCSprite* sp;
            if (speed > 0) {
                sp = CCSprite::create("GreenGrid.png");
            } else {
                sp = CCSprite::create("RedGrid.png");
            }
            speed --;
            TileMapWrapper::PathGrid pg(sp, *it);
            tmw->mPathGrids.push_back(pg);
            tmw->addChild(sp, 12);
        }
    }

    CCPoint last(c->x, c->y);
    std::list<TileMapWrapper::PathGrid>::iterator it;
    for (it = tmw->mPathGrids.begin(); it != tmw->mPathGrids.end(); it++) {
        CCPoint dp = ccpSub((*it).pt, last);
        last = (*it).pt;
        Direction d(dp.x, dp.y);
        mDirections.push_back(d);
    }

    // GameScene* scene = (GameScene*)this->getParent();
    // scene->setTouchEnabled(false);

    // mCurrentCreature = c;
    onMoveFinished(this);
    return true;
}/*}}}*/

cocos2d::CCPoint GameMapLayer::pointToMap(cocos2d::CCPoint point)
{/*{{{*/
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
}/*}}}*/

cocos2d::CCPoint GameMapLayer::mapToPoint(cocos2d::CCPoint point)
{/*{{{*/
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCPoint center;
    center.x = origin.x + visibleSize.width / 2;
    center.y = origin.y + visibleSize.height / 2;

    CCPoint dd;
    dd.x = (((MAPWIDTH + 1) % 2) / 2.0f) * 32;
    dd.y = (((MAPHEIGHT + 1) % 2) / 2.0f) * 32;


    CCPoint dp;
    dp = ccpSub(point, ccp(mGameMap.centerX, mGameMap.centerY));
    dp.x *= 32;
    dp.y *= 32;

    CCPoint pt;
    pt = ccpAdd(center, dp);
    pt = ccpAdd(pt, dd);

    return pt;
}/*}}}*/

void GameMapLayer::onTurn()
{/*{{{*/
    mCurrentTurn++;
    // RUN_HERE() << mCurrentTurn;

    tmw->removeAllPathGrids();
    tmw->showGrid(false);

    if ((mCurrentTurn % 2) == 0) {
        mCurrentCreature->onEndTurn(this);
        mCurrentCreature = nextCreature();
        if (mCurrentCreature)
            mCurrentCreature->onStartTurn(this);
    }
    if (mCurrentCreature == NULL) {
        mCurrentCreature = Hero::getInstance();
        mCurrentCreature->onStartTurn(this);
    }
    tmw->refreshMap();
    mCurrentCreature->onTurn(this);
}/*}}}*/

void GameMapLayer::onTurn(float delay)
{
    idle(delay);
}

void GameMapLayer::onTurnHelper(float dt)
{
    this->unschedule(schedule_selector(GameMapLayer::onTurnHelper));
    onTurn();
}

void GameMapLayer::idle(float delay)
{
    onTurn();
    // this->scheduleOnce(schedule_selector(GameMapLayer::onTurnHelper), delay);
}

Creature* GameMapLayer::nextCreature()
{/*{{{*/

    Hero* hero = Hero::getInstance();
    for (int x = hero->x - hero->senseRange(); x < hero->x + hero->senseRange(); x++) {
        for (int y = hero->y - hero->senseRange(); y < hero->y + hero->senseRange(); y++) {
            Creature* c = mGameMap.at(x, y)->creature;
            if (c != NULL) {
                addActiveCreature(c);
            }
        }
    }

    while (!mActiveCreatures.empty()) {
        Creature* c = mActiveCreatures.front();
        mActiveCreatures.pop_front();
        if (c != hero) {
            float dist = mGameMap.calcDistance(c->x, c->y, hero->x, hero->y);
            if (dist > c->senseRange()) {
                continue;
            }
            if (dist > c->sight()) {
                continue;
            }
        }
        mActiveCreatures.push_back(c);
        return c;
    }
    return NULL;
}/*}}}*/

void GameMapLayer::removeActiveCreature(Creature* c)
{/*{{{*/
    if (c == NULL)
        return;
    std::list<Creature*>::iterator it;
    it = std::find(mActiveCreatures.begin(), mActiveCreatures.end(), c);
    if (it != mActiveCreatures.end())
        mActiveCreatures.erase(it);
}/*}}}*/

void GameMapLayer::addActiveCreature(Creature* c)
{/*{{{*/
    if (c == NULL)
        return;
    std::list<Creature*>::iterator it;
    it = std::find(mActiveCreatures.begin(), mActiveCreatures.end(), c);
    if (it == mActiveCreatures.end())
        mActiveCreatures.push_back(c);
}/*}}}*/

bool GameMapLayer::isHeroTurn()
{
    return mCurrentCreature == Hero::getInstance();
}


