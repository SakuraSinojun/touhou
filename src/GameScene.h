

#pragma once

#include "cocos2d.h"
#include "hero.h"
#include <list>

class GameMapLayer;
class StatusLayer;

class GameScene;
class GameSceneMainLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(GameSceneMainLayer);
    virtual bool init();

    void onBackFromStatusLayer();

private:
    GameScene*          mGameLayer;
    StatusLayer*        mStatusLayer;

    cocos2d::CCMenuItemImage* pMove;
    cocos2d::CCMenuItemImage* pAttack;
    cocos2d::CCMenuItemImage* pExamine;
    cocos2d::CCMenuItemImage* pMagic;
    cocos2d::CCMenuItemImage* pStatus;
    cocos2d::CCMenuItemImage* pHero;
    cocos2d::CCMenuItemImage* pTurn;

    void onMenuMove(cocos2d::CCObject* pSender);
    void onMenuAttack(cocos2d::CCObject* pSender);
    void onMenuExamine(cocos2d::CCObject* pSender);
    void onMenuHero(cocos2d::CCObject* pSender);
    void onMenuStatus(cocos2d::CCObject* pSender);
    void onMenuMagic(cocos2d::CCObject* pSender);
    void onMenuTurn(cocos2d::CCObject* pSender);

    void onZoomIn(cocos2d::CCObject* pSender);
    void onZoomOut(cocos2d::CCObject* pSender);

};

class GameScene : public  cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();

    CREATE_FUNC(GameScene);

    GameMapLayer* gameMapLayer() { return mGameMapLayer; }

private:
    // cocos2d::CCTMXTiledMap* bg;
    // cocos2d::CCSprite*      hero;

    cocos2d::CCPoint mDragStartPoint;

    unsigned int        mClickTime;
    cocos2d::CCPoint    mClickPoint;

    GameMapLayer*       mGameMapLayer;

    // cocos2d::CCSprite*  mGrid;

    bool bMoving;

private:
    // for mobile
    float lastScale;
    float mScaleDistance;

    cocos2d::CCLabelTTF* pDebug;

private:
    void registerWithTouchDispatcher();

    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    void singleTouchDragging(cocos2d::CCPoint startPoint, cocos2d::CCPoint now);
    void onClick(cocos2d::CCPoint point);
    void onEnsureMove();

private:

    typedef void (GameScene::*WalkFunc)(void);
    std::list<WalkFunc> mWalkList;
    // std::list<cocos2d::CCSprite*> mPathGrids;
};

