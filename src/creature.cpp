
#include "creature.h"
#include "GameMapLayer.h"
#include "logging.h"

USING_NS_CC;

static int g_tag = 1000;

Creature::Creature()
    : mSprite(NULL)
    , mHpBar(new HpBar())
    , mCurrentHP(10)
    , mAI(NULL)
{
}

Creature::~Creature()
{
    if (mSprite)
        delete mSprite;
    if (mHpBar)
        delete mHpBar;
}

void Creature::Attach(CCSprite* cs)
{ 
    if (cs) {
        mSprite = cs;
    } 
}

CCSprite* Creature::getSprite()
{ 
    return mSprite; 
}

cocos2d::CCSprite* Creature::createSprite()
{
    mSprite = new CCSprite();
    mSprite->setTag(g_tag++);
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char* file)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file);
    mSprite->setTag(g_tag++);
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char * file, cocos2d::CCRect& rect)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file, rect);
    mSprite->setTag(g_tag++);
    return mSprite;
}

int Creature::currentHp()
{
    return mCurrentHP;
}

int Creature::maxHp()
{
    return 10;
}

int Creature::attackRange()
{
    return 1;
}

Creature& Creature::attack(Creature& o)
{
    o.mCurrentHP -= 3;
    if (o.mCurrentHP <= 0)
        o.mCurrentHP = 0;
    o.mHpBar->setPercent((float)o.mCurrentHP / o.maxHp());
    return *this;
}

Creature& Creature::attack(Creature* o)
{
    if (o)
        attack(*o);
    return *this;
}

int Creature::speed()
{
    return 5;
}

int Creature::sight()
{
    return 12;
}

int Creature::senseRange()
{
    return 20;
}

void Creature::onTurn(GameMapLayer* gml)
{
    if (mAI)
        mAI->turn();
    // gml->idle();
    cocos2d::CCPoint pt;
    pt.x = x + 1;
    pt.y = y;
    if (!gml->moveCurrentCreature(pt))
        gml->idle();
}

void Creature::onEndTurn(GameMapLayer* gml)
{
}

void Creature::StartWalkingAnimation(int dx, int dy)
{
}

void Creature::StopWalkingAnimation()
{
}


