
#include "GameScene.h"
#include "GameMapLayer.h"

#include "StatusLayer.h"

#include "logging.h"

USING_NS_CC;


#define MAPMOVETIMEPERGRID  0.15f
#define MENUZOOMPERIOD      0.1f

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

    /*
    CCMenuItemFont* pStatus = CCMenuItemFont::create("Status", this, menu_selector(GameSceneMainLayer::onMenuStatus));
    CCMenuItemFont* pMagic  = CCMenuItemFont::create("Magic", this, menu_selector(GameSceneMainLayer::onMenuMagic));
    CCMenuItemFont* pZoomIn = CCMenuItemFont::create("ZoomIn", this, menu_selector(GameSceneMainLayer::onZoomIn));
    CCMenuItemFont* pZoomOut= CCMenuItemFont::create("ZoomOut", this, menu_selector(GameSceneMainLayer::onZoomOut));

    CCMenu* menu = CCMenu::create(pStatus, pMagic, pZoomIn, pZoomOut, NULL);
    menu->alignItemsHorizontally();
    menu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + 32));
    this->addChild(menu, 200);
    */

    pMove = CCMenuItemImage::create("icons/move.png", "icons/move.png", this, menu_selector(GameSceneMainLayer::onMenuMove));
    pAttack = CCMenuItemImage::create("icons/attack.png", "icons/attack.png", this, menu_selector(GameSceneMainLayer::onMenuAttack));
    pExamine = CCMenuItemImage::create("icons/examine.png", "icons/examine.png", this, menu_selector(GameSceneMainLayer::onMenuExamine));
    pMagic = CCMenuItemImage::create("icons/magic.png", "icons/magic.png", this, menu_selector(GameSceneMainLayer::onMenuMagic));
    pStatus = CCMenuItemImage::create("icons/status.png", "icons/status.png", this, menu_selector(GameSceneMainLayer::onMenuStatus));
    pHero = CCMenuItemImage::create("icons/hero.png", "icons/hero.png", this, menu_selector(GameSceneMainLayer::onMenuHero));
    pTurn = CCMenuItemImage::create("icons/turn.png", "icons/turn.png", this, menu_selector(GameSceneMainLayer::onMenuTurn));
    CCMenu* menu = CCMenu::create(pMove, pAttack, pExamine, pMagic, pStatus, pHero, pTurn, NULL);
    menu->alignItemsVerticallyWithPadding(5);
    menu->setPosition(ccp(origin.x + visibleSize.width - pMove->getContentSize().width / 2 , origin.y + visibleSize.height / 2));
    this->addChild(menu, 200);

    pMove->setScale(1.5f);
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

void GameSceneMainLayer::onMenuMove(cocos2d::CCObject* pSender)
{
    CCScaleTo* ct1 = CCScaleTo::create(MENUZOOMPERIOD, 1.0f);
    CCScaleTo* ct2 = CCScaleTo::create(MENUZOOMPERIOD, 1.0f);
    CCScaleTo* ct3 = CCScaleTo::create(MENUZOOMPERIOD, 1.5f);
    pAttack->runAction(ct1);
    pExamine->runAction(ct2);
    pMove->runAction(ct3);

    mGameLayer->gameMapLayer()->setClickType(GameMapLayer::CT_MOVE);
}

void GameSceneMainLayer::onMenuAttack(cocos2d::CCObject* pSender)
{
    CCScaleTo* ct1 = CCScaleTo::create(MENUZOOMPERIOD, 1.5f);
    CCScaleTo* ct2 = CCScaleTo::create(MENUZOOMPERIOD, 1.0f);
    CCScaleTo* ct3 = CCScaleTo::create(MENUZOOMPERIOD, 1.0f);
    pAttack->runAction(ct1);
    pExamine->runAction(ct2);
    pMove->runAction(ct3);

    mGameLayer->gameMapLayer()->setClickType(GameMapLayer::CT_ATTACK);
}

void GameSceneMainLayer::onMenuExamine(cocos2d::CCObject* pSender)
{
    CCScaleTo* ct1 = CCScaleTo::create(MENUZOOMPERIOD, 1.0f);
    CCScaleTo* ct2 = CCScaleTo::create(MENUZOOMPERIOD, 1.5f);
    CCScaleTo* ct3 = CCScaleTo::create(MENUZOOMPERIOD, 1.0f);
    pAttack->runAction(ct1);
    pExamine->runAction(ct2);
    pMove->runAction(ct3);

    mGameLayer->gameMapLayer()->setClickType(GameMapLayer::CT_EXAMINE);
}

void GameSceneMainLayer::onMenuHero(cocos2d::CCObject* pSender)
{
}

void GameSceneMainLayer::onMenuTurn(cocos2d::CCObject* pSender)
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

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    mGameMapLayer = GameMapLayer::create();
    this->addChild(mGameMapLayer, 0);

    // Hero
    Hero::getInstance()->MoveTo(0, 0, mGameMapLayer->gamemap());

    // Map
    // mGameMapLayer->gamemap().at(0, 0)->creature = Hero::getInstance();
    mGameMapLayer->centerMap(ccp(0, 0));
   

    // DEBUG Message
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

void GameScene::onClick(cocos2d::CCPoint point)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    CCPoint np = mGameMapLayer->getPosition();
    point = ccpSub(point, np);

    CCPoint dp;
    dp.x = (visibleSize.width / 2 * (this->getScale() - 1) + point.x)/ this->getScale();
    dp.y = (visibleSize.height / 2 * (this->getScale() - 1) + point.y) / this->getScale();

    mGameMapLayer->onClick(dp);
}



