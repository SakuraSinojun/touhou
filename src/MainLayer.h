
#pragma once

#include "cocos2d.h"


class StatusLayer;
class MapLayer;
class MainLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(MainLayer);
    virtual bool init();

private:
    cocos2d::CCMenuItemImage* pMove;
    cocos2d::CCMenuItemImage* pSpellbook;
    cocos2d::CCMenuItemImage* pExamine;
    cocos2d::CCMenuItemImage* pMagic;
    cocos2d::CCMenuItemImage* pStatus;
    cocos2d::CCMenuItemImage* pHero;
    cocos2d::CCMenuItemImage* pTurn;

    StatusLayer* mStatusLayer;
    MapLayer*    mMapLayer;

    void onMenuMove(cocos2d::CCObject* pSender);
    void onMenuMagic(cocos2d::CCObject* pSender);
    void onMenuSpellbook(cocos2d::CCObject* pSender);
    void onMenuExamine(cocos2d::CCObject* pSender);
    void onMenuHero(cocos2d::CCObject* pSender);
    void onMenuStatus(cocos2d::CCObject* pSender);
    void onMenuTurn(cocos2d::CCObject* pSender);
    void onZoomIn(cocos2d::CCObject* pSender);
    void onZoomOut(cocos2d::CCObject* pSender);


public:
};








