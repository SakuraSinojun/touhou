
#pragma once

#include "cocos2d.h"

class StatusLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(StatusLayer);
    virtual bool init();

    void onShow();

private:
    void registerWithTouchDispatcher();
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

};

