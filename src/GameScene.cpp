
#include "GameScene.h"
#include "GameMapLayer.h"

#include "StatusLayer.h"

#include "logging.h"

USING_NS_CC;


#define MAPMOVETIMEPERGRID  0.15f


bool GameSceneMainLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
 
    mGameLayer = GameScene::create();
    this->addChild(mGameLayer);

    mStatusLayer = StatusLayer::create();
    mStatusLayer->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    mStatusLayer->setVisible(false);
    this->addChild(mStatusLayer, 100);

    CCMenuItemFont* pStatus = CCMenuItemFont::create("Status", this, menu_selector(GameSceneMainLayer::onMenuStatus));
    CCMenuItemFont* pMagic  = CCMenuItemFont::create("Magic", this, menu_selector(GameSceneMainLayer::onMenuMagic));
    CCMenuItemFont* pZoomIn = CCMenuItemFont::create("ZoomIn", this, menu_selector(GameSceneMainLayer::onZoomIn));
    CCMenuItemFont* pZoomOut= CCMenuItemFont::create("ZoomOut", this, menu_selector(GameSceneMainLayer::onZoomOut));

    CCMenu* menu = CCMenu::create(pStatus, pMagic, pZoomIn, pZoomOut, NULL);
    menu->alignItemsHorizontally();
    menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + 32));
    this->addChild(menu, 200);


    return true;
}

void GameSceneMainLayer::onMenuStatus(cocos2d::CCObject* pSender)
{
    if (mStatusLayer->isVisible()) {
        onBackFromStatusLayer();
    } else {
        mGameLayer->setTouchEnabled(false);
        mStatusLayer->setVisible(true);
        mStatusLayer->onShow();
    }
}

void GameSceneMainLayer::onMenuMagic(cocos2d::CCObject* pSender)
{
}

void GameSceneMainLayer::onZoomIn(cocos2d::CCObject* pSender)
{
    float s = mGameLayer->getScale();
    s += 0.1;
    if (s >= 2.0f)
        s = 2.0f;
    mGameLayer->setScale(s);
}

void GameSceneMainLayer::onZoomOut(cocos2d::CCObject* pSender)
{
    float s = mGameLayer->getScale();
    s -= 0.1;
    if (s <= 1.0f)
        s = 1.0f;
    mGameLayer->setScale(s);
}

void GameSceneMainLayer::onBackFromStatusLayer()
{
    mGameLayer->setTouchEnabled(true);
    mStatusLayer->setVisible(false);
}




CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    GameSceneMainLayer* layer = GameSceneMainLayer::create();
    scene->addChild(layer);
    return scene;
}


bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    static int g_tag = 1000;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // bg = CCTMXTiledMap::create("test.tmx");
    // this->addChild(bg, 0);

    mGameMapLayer = GameMapLayer::create();
    this->addChild(mGameMapLayer, 0);

    CCRect  rect(0, 0, 64, 64);
    CCSprite* s = CCSprite::create("hero.png", rect);
    s->setTag(g_tag++);
    // s->initWithFile("hero.png", rect);
    CCPoint anchor = s->getAnchorPoint();
    RUN_HERE() << "anchor = (" << anchor.x << ", " << anchor.y << ")";
    anchor.y -= 16.0f / 64.0f;
    s->setAnchorPoint(anchor);
    mHero.Attach(s);
    mGameMapLayer->gamemap().at(mHero.x, mHero.y)->creature = &mHero;

    mGameMapLayer->centerMap(ccp(mHero.x, mHero.y));
    /*
    hero = CCSprite::create("hero.png", rect);
    CCPoint pt = ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    pt = pointToGrid(pt);
    pt = gridToPoint(pt);
    pt.y += 16;

    hero->setPosition(pt);
    this->addChild(hero, 10);
    */

    /*

    mGrid = CCSprite::create("RedGrid.png");
    mGrid->setPosition(ccp(0, 0));
    mGrid->setVisible(false);
    this->addChild(mGrid, 11);
    */
   
    pDebug = CCLabelTTF::create("DEBUG", "Marker Felt", 32);
    pDebug->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    pDebug->setVisible(false);
    this->addChild(pDebug, 300);


    mClickTime = 0;
    bMoving = false;

    // this->setScale(1.5f);

    lastScale = 0.0f;
    mScaleDistance = 0.0f;

    this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
 
    return true;
}


void GameScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void GameScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch* touch = (CCTouch*)(pTouches->anyObject());
    mDragStartPoint = touch->getLocation();
    mClickTime = time(NULL);
    mClickPoint = touch->getLocation();
}

void GameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (pTouches->count() == 2) {

        CCSetIterator it1 = pTouches->begin();
        CCSetIterator it2 = it1;
        it2++;
        CCPoint pt1 = ((CCTouch*)(*it1))->getLocation();
        CCPoint pt2 = ((CCTouch*)(*it2))->getLocation();
        float dist = sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) + (pt1.y - pt2.y) * (pt1.y - pt2.y));

        if (lastScale < 1.0f || mScaleDistance == 0.0f) {
            lastScale = this->getScale();
            mScaleDistance = dist;
        }

        float scale =  lastScale * (dist / mScaleDistance);

        { 
            char    temp[4096];
            snprintf(temp, sizeof(temp), "%f/%f/%f", dist, mScaleDistance, scale);
            pDebug->setString(temp);
        }

        if (scale < 1.0f)
            scale = 1.0f;
        if (scale > 2.0f)
            scale = 2.0f;

        lastScale = scale;
        mScaleDistance = dist;

        this->setScale(scale);
    } else {
        CCTouch* touch = (CCTouch*)(pTouches->anyObject());
        singleTouchDragging(mDragStartPoint, touch->getLocation());
        mDragStartPoint = touch->getLocation();
    }
}

void GameScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint pt = touch->getLocation();
    CCPoint d = ccpSub(pt, mClickPoint);
    if (fabs(d.x) < 2.0f && fabs(d.y) < 2.0f) {
        onClick(pt);
    }

    lastScale = 0.0f;
    mScaleDistance = 0.0f;
    pDebug->setString("END");
}

void GameScene::singleTouchDragging(CCPoint startPoint, CCPoint now)
{
    CCPoint d = ccpSub(now, startPoint);
    d.x /= this->getScale();
    d.y /= this->getScale();
    CCPoint n = ccpAdd(mGameMapLayer->getPosition(), d);

    // CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    // RUN_HERE() << "width = " << mGameMap->getContentSize().width;

    CCPoint center = mGameMapLayer->getMapCenter();
    if (n.x > origin.x + 32) {
        n.x = origin.x;
        center.x--;
        mGameMapLayer->centerMap(center);
    }
    if (n.y > origin.y + 32) {
        n.y = origin.y;
        center.y--;
        mGameMapLayer->centerMap(center);
    }
    if (n.x < origin.x - 32) {
        n.x = origin.x;
        center.x++;
        mGameMapLayer->centerMap(center);
    }
    if (n.y < origin.y - 32) {
        n.y = origin.y;
        center.y++;
        mGameMapLayer->centerMap(center);
    }
    mGameMapLayer->setPosition(n);
}

class CHelper {
public:
    void operator() (int x, int y) {
        // RUN_HERE() << "x = " << x << " y = " << y;
        CCPoint pt(x, y);
        nodes.push_back(pt);
    }
    std::list<CCPoint> nodes;
};

void GameScene::onClick(cocos2d::CCPoint point)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    CCPoint np = mGameMapLayer->getPosition();
    point = ccpSub(point, np);

    CCPoint dp;
    dp.x = (visibleSize.width / 2 * (this->getScale() - 1) + point.x)/ this->getScale();
    dp.y = (visibleSize.height / 2 * (this->getScale() - 1) + point.y) / this->getScale();

    mGameMapLayer->onClick(dp);
    return;

#if 0
    // this->runAction(CCScaleTo::create(0.5, 1.5f));
    // this->setScale(1.5f);

    point.x = (point.x + (this->getScale() - 1) * visibleSize.width / 2.0f) / this->getScale();
    point.y = (point.y + (this->getScale() - 1) * visibleSize.height / 2.0f) / this->getScale();

    if (bMoving)
        return;

    static CCPoint lastGrid(-10, -10);
    CCPoint pt = pointToGrid(point);
    if ((pt.x == lastGrid.x) && (pt.y == lastGrid.y)) {
        onEnsureMove();
        return;
    }
    lastGrid = pt;

    CCPoint cl = gridToMap(pt);
    // RUN_HERE() << "Hero : (" << mHero.x << ", " << mHero.y << ")";
    // RUN_HERE() << "Click: (" << cl.x << ", " << cl.y << ")";

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        this->removeChild(*it);
    }
    mPathGrids.clear();

    CHelper ch;
    if (mGameMapLayer->findPath(ccp(mHero.x, mHero.y), cl, ch)) {
        hideGrid();
        std::list<CCPoint>::iterator nit;
        for (nit = ch.nodes.begin(); nit != ch.nodes.end(); nit++) {
            if (nit == ch.nodes.begin())
                continue;

            // RUN_HERE() << "(" << (*nit).x << ", " << (*nit).y << ")";
            CCSprite* sp = CCSprite::create("GreenGrid.png");
            CCPoint pt = gridToPoint(mapToGrid(*nit));
            sp->setPosition(pt);
            sp->setVisible(true);
            this->addChild(sp, 11);
            mPathGrids.push_back(sp);
        }
    } else {
        showGrid(pt);
    }
#endif
}

void GameScene::onEnsureMove()
{
    if (mPathGrids.empty())
        return;

    CCPoint last(mHero.x, mHero.y);
    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        CCPoint pt = gridToMap(pointToGrid((*it)->getPosition()));
        CCPoint dpt = ccpSub(pt, last);
        last = pt;
        if (dpt.x > 0) {
            if (dpt.y > 0) {
                mWalkList.push_back(&GameScene::WalkNorthEast);
            } else if (dpt.y < 0) {
                mWalkList.push_back(&GameScene::WalkSouthEast);
            } else {
                mWalkList.push_back(&GameScene::WalkEast);
            }
        } else if (dpt.x < 0) {
            if (dpt.y > 0) {
                mWalkList.push_back(&GameScene::WalkNorthWest);
            } else if (dpt.y < 0) {
                mWalkList.push_back(&GameScene::WalkSouthWest);
            } else {
                mWalkList.push_back(&GameScene::WalkWest);
            }
        } else {
            if (dpt.y > 0)
                mWalkList.push_back(&GameScene::WalkNorth);
            else
                mWalkList.push_back(&GameScene::WalkSouth);
        }
    }
    onMapMoveFinished(this);
}

CCPoint GameScene::pointToGrid(cocos2d::CCPoint point)
{
    CCPoint pt;
    pt.x = floor(point.x / 32);
    pt.y = floor(point.y / 32);
    return pt;
}

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

void GameScene::WalkSouth(void)
{/*{{{*/
    /*
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("hero.png");
    float w = texture->getContentSize().width / 4.0f;
    float h = texture->getContentSize().width / 4.0f;
    CCAnimation* animation = CCAnimation::create();
    for (int i=0; i < 2; i++) {
        animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
    }
    animation->setDelayPerUnit(MAPMOVETIMEPERGRID / 2.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    hero->runAction(CCRepeatForever::create(animate));
    */
    mHero.move(0, -1);

    CCPoint pt(0, 32);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.y += 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.y += 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }
}/*}}}*/

void GameScene::WalkNorth(void)
{/*{{{*/
    mHero.move(0, 1);

    CCPoint pt(0, -32);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.y -= 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.y -= 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }

}/*}}}*/

void GameScene::WalkWest(void)
{/*{{{*/
    mHero.move(-1, 0);

    CCPoint pt(32, 0);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.x += 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.x += 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }

}/*}}}*/

void GameScene::WalkEast(void)
{/*{{{*/
    mHero.move(1, 0);

    CCPoint pt(-32, 0);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.x -= 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.x -= 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }


}/*}}}*/

void GameScene::WalkSouthEast(void)
{/*{{{*/
    mHero.move(1, -1);

    CCPoint pt(-32, 32);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.x -= 32;
    pt.y += 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.x -= 32;
        pt.y += 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }



}/*}}}*/

void GameScene::WalkSouthWest(void)
{/*{{{*/
    mHero.move(-1, -1);

    CCPoint pt(32, 32);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.x += 32;
    pt.y += 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.x += 32;
        pt.y += 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }
}/*}}}*/

void GameScene::WalkNorthEast(void)
{/*{{{*/
    mHero.move(1, 1);

    CCPoint pt(-32, -32);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.x -= 32;
    pt.y -= 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.x -= 32;
        pt.y -= 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }
}/*}}}*/

void GameScene::WalkNorthWest(void)
{/*{{{*/
    mHero.move(-1, 1);

    CCPoint pt(32, -32);
    CCMoveTo* mt = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(GameScene::onMapMoveFinished));
    mGameMapLayer->runAction(CCSequence::create(mt, cf, NULL));

    bMoving = true;

    /*
    pt = mGrid->getPosition();
    pt.x += 32;
    pt.y -= 32;
    CCMoveTo* mt1 = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
    mGrid->runAction(mt1);
    */

    std::list<cocos2d::CCSprite*>::iterator it;
    for (it = mPathGrids.begin(); it != mPathGrids.end(); it++) {
        pt = (*it)->getPosition();
        pt.x += 32;
        pt.y -= 32;
        CCMoveTo* m = CCMoveTo::create(MAPMOVETIMEPERGRID, pt);
        (*it)->runAction(m);
    }
}/*}}}*/


void GameScene::showGrid(cocos2d::CCPoint grid)
{
    /*
    CCPoint pt = gridToPoint(grid);
    mGrid->setPosition(pt);
    mGrid->setVisible(true);
    */
}

void GameScene::hideGrid()
{
    // mGrid->setVisible(false);
}

void GameScene::onMapMoveFinished(cocos2d::CCNode* sender)
{
    mGameMapLayer->setPosition(ccp(0, 0));
    mGameMapLayer->centerMap(ccp(mHero.x, mHero.y));
    // hero->stopAllActions();

    if (!mPathGrids.empty()) {
        this->removeChild(mPathGrids.front());
        mPathGrids.pop_front();
    }
 
    if (mWalkList.size() > 0) {
        std::list<WalkFunc>::iterator it = mWalkList.begin();
        WalkFunc fn = (WalkFunc)*it;
        mWalkList.erase(it);
        (this->*fn)();
    } else {
        bMoving = false;
    }
}


