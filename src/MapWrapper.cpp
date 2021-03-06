
#include "MapWrapper.h"
#include "GameResource.h"
#include "gamemap.h"
#include "creature.h"
#include "hero.h"
#include "logging.h"

USING_NS_CC;

void ProjectileCallback::onProjectileFinished()
{
    if (mCreature != NULL) {
        mCreature->onAttackFinished(NULL);
    }
}

bool MapWrapper::init()
{
    if (!CCLayer::init())
        return false;

    int i, j;
    for (i = 0; i < MAPWIDTH; i++) {
        for (j = 0; j < MAPHEIGHT; j++) {
            tiles[i][j] = NULL;
        }
    }

    mGrid = CCSprite::create("GreenGrid.png");
    mGrid->setVisible(false);
    this->addChild(mGrid, 11);



    refresh();
    return true;
}

void MapWrapper::showGrid(bool bShow)
{
    mIsGridVisible = bShow;
}

void MapWrapper::useRedGrid()
{
    this->removeChild(mGrid);
    mGrid = CCSprite::create("RedGrid.png");
    this->addChild(mGrid, 11);
}

void MapWrapper::useGreenGrid()
{
    this->removeChild(mGrid);
    mGrid = CCSprite::create("GreenGrid.png");
    this->addChild(mGrid, 11);
}

void MapWrapper::useYellowGrid()
{
    this->removeChild(mGrid);
    mGrid = CCSprite::create("YellowGrid.png");
    this->addChild(mGrid, 11);
}

void MapWrapper::refresh()
{
    GameResource* gr = GameResource::getInstance();
    Hero* hero = gr->hero();
    GameMap* gamemap = gr->gameMap();

    int i, j;
    int sx = gamemap->centerX - MAPWIDTH / 2;
    int sy = gamemap->centerY - MAPHEIGHT / 2;
    // RUN_HERE() << ccp(sx, sy);
    
    int nx = MAPWIDTH - (DESIGNWIDTH / TILESIZE);
    int ny = MAPHEIGHT - (DESIGNHEIGHT / TILESIZE);
    nx /= 2;
    ny /= 2;

    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    GameMap::Node* heronode = gamemap->at(hero->x, hero->y);

    // this->removeAllChildren();
    CCObject* node = NULL;
    CCARRAY_FOREACH(this->getChildren(), node)
    {
        CCNode* n = (CCNode*)node;
        if (n->getTag() > 0) {
            n->setVisible(false);
        }
    }
    
    for (i = 0; i < MAPWIDTH; i++) {
        for (j = 0; j < MAPHEIGHT; j++) {
            int x = sx + i;
            int y = sy + j;
            // RUN_HERE() << "(" << x << ", " << y << ")";

            GameMap::Node* n = gamemap->at(x, y);
            if (!tiles[i][j]) {
                tiles[i][j] = MapTile::create();
                this->addChild(tiles[i][j], 0);
            }
            int cx = origin.x + (i - nx) * TILESIZE + TILESIZE / 2;
            int cy = origin.y + (j - ny) * TILESIZE + TILESIZE / 2;
            tiles[i][j]->setPosition(ccp(cx, cy));
            tiles[i][j]->setType(n->type);

#if 0
            // 探索视野
            // TODO 不应该在这里
            if (!n->explored) {
                if (gamemap->isNodeCanBeSeen(n, heronode)) {
                    n->explored = true;
                }
            }

            tiles[i][j]->setVisible(n->explored);
#else
            // 正常视野
            tiles[i][j]->setVisible(gamemap->isNodeCanBeSeen(n, heronode));
#endif

            if (n->creature != NULL) {
                CCSprite* s = n->creature->getSprite();
                CCSprite* b = n->creature->getBar();
                if (s != NULL) {
                    s->setPosition(ccp(cx, cy));
                    b->setPosition(ccp(cx, cy + TILESIZEF / 2.0f));
                    s->setVisible(tiles[i][j]->isVisible());
                    b->setVisible(tiles[i][j]->isVisible());

                    if (this->getChildByTag(s->getTag()) == NULL) {
                        this->addChild(s, (s->getTag() == 1000) ? 11 : 10);
                        this->addChild(b, (s->getTag() == 1000) ? 11 : 10);
                    }
                }
            }
            if (n->ornament != NULL) {
                CCSprite* s = n->ornament->Sprite();
                if (s != NULL) {
                    s->setPosition(ccp(cx, cy));
                    s->setVisible(tiles[i][j]->isVisible());
                    if (this->getChildByTag(s->getTag()) == NULL) {
                        this->addChild(s, 1);
                    }
                }
            }
            if (!n->items.empty()) {
                Item* item = n->items.front();
                CCSprite* s = item->Sprite();
                if (s != NULL) {
                    s->setPosition(ccp(cx, cy));
                    s->setVisible(tiles[i][j]->isVisible());
                    if (this->getChildByTag(s->getTag()) == NULL) {
                        this->addChild(s, 2);
                    }
                }
            }
        }
    }
    if (mGridPosition.x > sx && mGridPosition.x < sx + MAPWIDTH && mGridPosition.y > sy && mGridPosition.y < sy + MAPHEIGHT) {
        int x = mGridPosition.x - sx;
        int y = mGridPosition.y - sy;
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        mGrid->setPosition(ccp(origin.x + (x - nx) * 32 + 16, origin.y + (y - ny) * 32 + 16));
        mGrid->setVisible(mIsGridVisible);
    }
}

CCPoint MapWrapper::mapToPoint(CCPoint point)
{
    GameMap* gamemap = GameResource::getInstance()->gameMap();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCPoint center;
    center.x = origin.x + visibleSize.width / 2;
    center.y = origin.y + visibleSize.height / 2;

    CCPoint dd;
    dd.x = (((MAPWIDTH + 1) % 2) / 2.0f) * TILESIZE;
    dd.y = (((MAPHEIGHT + 1) % 2) / 2.0f) * TILESIZE;

    CCPoint dp;
    dp = ccpSub(point, ccp(gamemap->centerX, gamemap->centerY));
    dp.x *= TILESIZE;
    dp.y *= TILESIZE;
    
    CCPoint pt;
    pt = ccpAdd(center, dp);
    pt = ccpAdd(pt, dd);

    return pt;
}

bool MapWrapper::addProjectile(CCNode* prj, CCPoint start, CCPoint end, float duration, ProjectileCallback* fp)
{
    Projectile * pj = new Projectile(prj, fp);

    this->addChild(prj);
    prj->setPosition(mapToPoint(start));
    CCMoveTo* ct = CCMoveTo::create(duration, mapToPoint(end));
    CCCallFuncND* cf = CCCallFuncND::create(this, callfuncND_selector(MapWrapper::onProjectileMoveFinished), pj);
    prj->runAction(CCSequence::create(ct, cf, NULL));
    return true;
}

void MapWrapper::onProjectileMoveFinished(cocos2d::CCNode* pSender, void* data)
{
    Projectile * pj = (Projectile*)data;
    if (pj->mProjectile)
        this->removeChild(pj->mProjectile);
    if (pj->mProjectileCallback != NULL) {
        pj->mProjectileCallback->onProjectileFinished();
    }
}





