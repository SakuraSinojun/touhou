

#pragma once
#include "cocos2d.h"

class MainLayer;
class PlayingScene : public cocos2d::CCScene
{
public:
    CREATE_FUNC(PlayingScene);
    virtual bool init();
private:
    MainLayer* mMainLayer;
};



