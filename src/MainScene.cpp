
#include "MainScene.h"
#include "HeroCreator.h"
#include "PlayingScene.h"
#include "logging.h"

USING_NS_CC;


CCScene* MainScene::scene()
{
    CCScene* scene = CCScene::create();
    MainScene* layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}


bool MainScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCSprite* pBackground = CCSprite::create();
    pBackground->initWithFile("Titles/title.jpg");
    pBackground->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    pBackground->setVisible(false);
    this->addChild(pBackground, 0);

    CCMenuItemFont * start = CCMenuItemFont::create("Play", this, menu_selector(MainScene::onPlayButtonClicked));
    CCMenuItemFont * setup = CCMenuItemFont::create("Setup", this, menu_selector(MainScene::onSetupButtonClicked));
    CCMenu* menu = CCMenu::create(start, setup, NULL);
    this->addChild(menu);
    menu->alignItemsVertically();

    // this->setTouchEnabled(true);
    // CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);

    return true;
}


void MainScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void MainScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    // CCTouch* touch = (CCTouch*)(touches->anyObject());
    // CCPoint pt = touch->getLocation();
    // RUN_HERE() << "touch: (" << pt.x << ", " << pt.y << ")";
}

void MainScene::onPlayButtonClicked(CCObject* pSender)
{
    // CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, HeroCreator::scene()));
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, PlayingScene::create()));
}

void MainScene::onSetupButtonClicked(CCObject* pSender)
{
    // CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, HeroCreator::scene()));
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, PlayingScene::create()));
}


