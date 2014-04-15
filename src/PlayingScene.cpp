
#include "PlayingScene.h"
#include "MainLayer.h"

#include "logging.h"

USING_NS_CC;



bool PlayingScene::init()
{
    if (!CCScene::init())
        return false;
    mMainLayer = MainLayer::create();
    this->addChild(mMainLayer);
    return true;
}


