
#include "GameOverScene.h"
#include "MainScene.h"
#include "GameResource.h"

USING_NS_CC;


bool GameOverScene::init()
{
    if (!CCScene::init())
        return false;
    GameOverLayer* layer = GameOverLayer::create();
    this->addChild(layer);
    return true;
}

bool GameOverLayer::init()
{
    if (!CCLayer::init())
        return false;

    CCMenuItemFont* pGameOver = CCMenuItemFont::create("GameOver", this, menu_selector(GameOverLayer::onMenuGameOver));
    CCMenu* menu = CCMenu::create(pGameOver, NULL);
    this->addChild(menu);

    return true;
}

void GameOverLayer::onMenuGameOver(CCObject* pSender)
{
    GameResource::getInstance()->reset();
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.3f, MainScene::scene()));
}


