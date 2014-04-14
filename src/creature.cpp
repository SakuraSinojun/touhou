
#include "creature.h"
#include "GameMapLayer.h"
#include "logging.h"

USING_NS_CC;

static int g_tag = 1000;

Creature::Creature()
    : mSprite(NULL)
    , mHpBar(new HpBar())
    , mAI(NULL)
    , mMaxSpeed(5)
    , mSpeed(5)
    , mLevel(1)
{
    mCurrentHP = maxHp();
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
    return 10 * level();
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

int Creature::sight()
{
    return 12;
}

int Creature::senseRange()
{
    return 20;
}

void Creature::onStartTurn(GameMapLayer* gml)
{
    mSpeed = mMaxSpeed;
}

void Creature::onTurn(GameMapLayer* gml)
{
    if (mAI)
        mAI->turn(gml);
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

int Creature::expOnDeath()
{
    return mLevel * 10;
}

int Creature::exp()
{
    return mExp;
}

int Creature::addExp(int d)
{
    mExp += d;
    int ol = mLevel;
    mLevel = mExp / 20 + 1;
    if (mLevel != ol) {
        this->mCurrentHP = maxHp();
    }
    return mLevel;
}

int Creature::level()
{
    return mLevel;
}



