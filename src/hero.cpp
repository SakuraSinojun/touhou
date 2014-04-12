

#include "hero.h"
#include "GameMapLayer.h"
#include "cocos2d.h"
USING_NS_CC;

static Hero* g_hero;

Hero* Hero::getInstance()
{
    if (!g_hero)
        g_hero = new Hero();
    return g_hero;
}

Hero::Hero()
    : Creature()
{
    CCRect  rect(0, 0, 32, 48);
    CCSprite* s = createSprite("hero.png", rect);
    CCPoint anchor = s->getAnchorPoint();
    anchor.y -= 16.0f / 48.0f;
    s->setAnchorPoint(anchor);
}

Hero::~Hero()
{
}

void Hero::turn()
{
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


