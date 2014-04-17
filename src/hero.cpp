

#include "hero.h"
#include "gamemap.h"
#include "MapLayer.h"
#include "MapWrapper.h"
#include "GameScene.h"
#include "GameResource.h"
#include "cocos2d.h"
USING_NS_CC;

Hero::Hero()
    : Creature()
{
    CCRect  rect(0, 0, 32, 32);
    createSprite("hero.png", rect);
    // CCPoint anchor = getSprite()->getAnchorPoint();
    // anchor.y -= 16.0f / 48.0f;
    // getSprite()->setAnchorPoint(anchor);
    this->mCurrentHP = maxHp();
    // mProjectileHelper.setAttacker(this);
}

Hero::~Hero()
{
}

/*
void Hero::onStartTurn(GameMapLayer* gml)
{
    Creature::onStartTurn(gml);
    mCurrentHP += maxHp() / 10;
    if (mCurrentHP > maxHp()) {
        mCurrentHP = maxHp();
    }
    mHpBar->setPercent((float)mCurrentHP / maxHp());
}

void Hero::onTurn(GameMapLayer* gml)
{
    gml->centerMap(ccp(x, y));
    GameScene* s = (GameScene*)gml->getParent();
    s->setTouchEnabled(true);
}

void Hero::onEndTurn(GameMapLayer* gml)
{
    Creature::onEndTurn(gml);
    GameScene* s = (GameScene*)gml->getParent();
    s->setTouchEnabled(false);
}
*/

void Hero::StartWalkingAnimation(int dx, int dy)
{
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("hero.png");

    float w = texture->getContentSize().width / 3.0f;
    float h = texture->getContentSize().height / 4.0f;
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 4; i++) {
        animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
    }
    animation->setDelayPerUnit(HEROMOVEPERIOD / 3.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    getSprite()->runAction(CCRepeatForever::create(animate));

}

void Hero::StopWalkingAnimation()
{
    getSprite()->stopAllActions();
    getBar()->stopAllActions();
}

bool Hero::attackAnimate(Creature& o, MapLayer* gml)
{
    return fireBall(o, gml);
}

int Hero::sight()
{
    return 20;
}

int Hero::senseRange()
{
    return 20;
}

int Hero::maxHp()
{
    return 30 * level();
}


