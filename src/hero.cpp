

#include "hero.h"
#include "GameMapLayer.h"
#include "GameScene.h"
#include "cocos2d.h"
USING_NS_CC;

static Hero* g_hero;

Hero* Hero::getInstance()
{
    if (!g_hero)
        g_hero = new Hero();
    return g_hero;
}

void Hero::destroyHero()
{
    if (g_hero)
        delete g_hero;
    g_hero = NULL;
}

Hero::Hero()
    : Creature()
{
    CCRect  rect(0, 0, 32, 48);
    CCSprite* s = createSprite("hero.png", rect);
    CCPoint anchor = s->getAnchorPoint();
    anchor.y -= 16.0f / 48.0f;
    s->setAnchorPoint(anchor);
    this->mCurrentHP = maxHp();
}

Hero::~Hero()
{
}

void Hero::onStartTurn(GameMapLayer* gml)
{
    mCurrentHP += maxHp() / 10;
    if (mCurrentHP > maxHp()) {
        mCurrentHP = maxHp();
    }
    mHpBar->setPercent(mCurrentHP / maxHp());
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

void Hero::StartWalkingAnimation(int dx, int dy)
{
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("hero.png");

    float w = texture->getContentSize().width / 4.0f;
    float h = texture->getContentSize().height / 4.0f;
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 4; i++) {
        animation->addSpriteFrameWithTexture(texture, CCRectMake(i * w, 0, w, h));
    }
    animation->setDelayPerUnit(MAPMOVETIMEPERGRID / 4.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    getSprite()->runAction(CCRepeatForever::create(animate));

}

void Hero::StopWalkingAnimation()
{
    getSprite()->stopAllActions();
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


