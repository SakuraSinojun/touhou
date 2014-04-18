
#include "MainLayer.h"
#include "StatusLayer.h"
#include "MapLayer.h"
#include "GameResource.h"
#include "hero.h"
#include "gamemap.h"
#include "GameResource.h"

USING_NS_CC;


bool MainLayer::init()
{
    if (!CCLayer::init())
        return false;

    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();
    hero->MoveTo(0, 0, gamemap);
    gamemap->centerMap(0, 0);


    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    pMove = CCMenuItemImage::create("icons/move.png", "icons/move.png", this, menu_selector(MainLayer::onMenuMove));
    pMagic = CCMenuItemImage::create("icons/magic.png", "icons/magic.png", this, menu_selector(MainLayer::onMenuMagic));
    pSpellbook = CCMenuItemImage::create("icons/attack.png", "icons/attack.png", this, menu_selector(MainLayer::onMenuSpellbook));
    pExamine = CCMenuItemImage::create("icons/examine.png", "icons/examine.png", this, menu_selector(MainLayer::onMenuExamine));
    pStatus = CCMenuItemImage::create("icons/status.png", "icons/status.png", this, menu_selector(MainLayer::onMenuStatus));
    pHero = CCMenuItemImage::create("icons/hero.png", "icons/hero.png", this, menu_selector(MainLayer::onMenuHero));
    pTurn = CCMenuItemImage::create("icons/turn.png", "icons/turn.png", this, menu_selector(MainLayer::onMenuTurn));
    CCMenu* menu = CCMenu::create(pMove, pMagic, pSpellbook, pExamine, pStatus, pHero, pTurn, NULL);
    menu->alignItemsVerticallyWithPadding(5);
    menu->setPosition(ccp(origin.x + visibleSize.width - pMove->getContentSize().width / 2 , origin.y + visibleSize.height / 2));
    menu->setVisible(true);
    pMove->setScale(1.5f);
    this->addChild(menu, 200);

    mStatusLayer = StatusLayer::create();
    mStatusLayer->setVisible(false);
    this->addChild(mStatusLayer, 100);

    mMapLayer = MapLayer::create();
    this->addChild(mMapLayer, 0);


    return true;
}

void MainLayer::onMenuMove(cocos2d::CCObject* pSender)
{
    pMagic->setScale(1.0f);
    pMove->setScale(1.5f);
    GameResource::getInstance()->setAction(GameResource::CT_MOVE);
}

void MainLayer::onMenuSpellbook(cocos2d::CCObject* pSender)
{
}

void MainLayer::onMenuExamine(cocos2d::CCObject* pSender)
{
}

void MainLayer::onMenuHero(cocos2d::CCObject* pSender)
{
    if (mMapLayer->isVisible()) {
        mMapLayer->setTouchEnabled(false);
        mStatusLayer->setTouchEnabled(true);
        mStatusLayer->setVisible(true);
        mMapLayer->setVisible(false);
    } else {
        mStatusLayer->setTouchEnabled(false);
        mMapLayer->setTouchEnabled(true);
        mStatusLayer->setVisible(false);
        mMapLayer->setVisible(true);
    }
}

void MainLayer::onMenuStatus(cocos2d::CCObject* pSender)
{
}

void MainLayer::onMenuMagic(cocos2d::CCObject* pSender)
{
    pMagic->setScale(1.5f);
    pMove->setScale(1.0f);
    GameResource::getInstance()->setAction(GameResource::CT_MAGIC);
}

void MainLayer::onMenuTurn(cocos2d::CCObject* pSender)
{
}

void MainLayer::onZoomIn(cocos2d::CCObject* pSender)
{
}

void MainLayer::onZoomOut(cocos2d::CCObject* pSender)
{
}














