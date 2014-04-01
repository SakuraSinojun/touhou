

#include "StatusLayer.h"
#include "GameScene.h"

#include "logging.h"

USING_NS_CC;


bool StatusLayer::init()
{
    if (!CCLayer::init())
        return false;
    CCSprite* sp = CCSprite::create("Titles/title.jpg");
    this->addChild(sp);

    // this->setTouchEnabled(true);
    return true;
}

void StatusLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void StatusLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    RUN_HERE();
    GameScene* s = (GameScene*)(this->getParent());
    s->onBackFromStatusLayer();
}

void StatusLayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
}

void StatusLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
}

void StatusLayer::onShow()
{
    this->setTouchEnabled(true);
}


