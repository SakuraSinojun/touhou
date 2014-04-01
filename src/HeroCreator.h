

#pragma once

#include "cocos2d.h"

class HeroCreator : public  cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();

    CREATE_FUNC(HeroCreator);

private:
    // menu
    void onAliceSelected(CCObject* pSender);

private:
    void registerWithTouchDispatcher();
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};

