
#pragma once

#include "cocos2d.h"


class GameOverScene : public cocos2d::CCScene
{
public:
    CREATE_FUNC(GameOverScene);
    virtual bool init();
};

class GameOverLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(GameOverLayer);
    virtual bool  init();

    void onMenuGameOver(cocos2d::CCObject* pSender);
};


