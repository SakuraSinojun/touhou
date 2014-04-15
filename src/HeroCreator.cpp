
#include "HeroCreator.h"
// #include "GameScene.h"
#include "PlayingScene.h"
#include "logging.h"

USING_NS_CC;


CCScene* HeroCreator::scene()
{
    CCScene* scene = CCScene::create();
    HeroCreator* layer = HeroCreator::create();
    scene->addChild(layer);
    return scene;
}


bool HeroCreator::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    // CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    // CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCMenuItemImage* pAlice = CCMenuItemImage::create("Pictures/h101.png", "Pictures/h102.png", this, menu_selector(HeroCreator::onAliceSelected));
    CCMenuItemImage* pFlandre = CCMenuItemImage::create("Pictures/h201.png", "Pictures/h202.png", this, menu_selector(HeroCreator::onAliceSelected));
    CCMenuItemImage* pHoanMeirin = CCMenuItemImage::create("Pictures/h301.png", "Pictures/h302.png", this, menu_selector(HeroCreator::onAliceSelected));
    CCMenuItemImage* pReimu = CCMenuItemImage::create("Pictures/h401.png", "Pictures/h402.png", this, menu_selector(HeroCreator::onAliceSelected));
    pAlice->setScale(0.5);
    pFlandre->setScale(0.5);
    pHoanMeirin->setScale(0.5);
    pReimu->setScale(0.5);
    CCMenu* menu = CCMenu::create(pAlice, pFlandre, pHoanMeirin, pReimu, NULL);
    menu->alignItemsHorizontally();

    this->addChild(menu);


    // this->setTouchEnabled(true);
    // CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);

    return true;
}


void HeroCreator::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void HeroCreator::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    // CCTouch* touch = (CCTouch*)(touches->anyObject());
    // CCPoint pt = touch->getLocation();
    // RUN_HERE() << "touch: (" << pt.x << ", " << pt.y << ")";
}

void HeroCreator::onAliceSelected(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, PlayingScene::create()));
}




