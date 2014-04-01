

#pragma once

#include "cocos2d.h"

class MainScene : public  cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();

    CREATE_FUNC(MainScene);


private:
    // Menu
    void onPlayButtonClicked(CCObject* pSender);
    void onSetupButtonClicked(CCObject* pSender);

private:

    void registerWithTouchDispatcher();
    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};

