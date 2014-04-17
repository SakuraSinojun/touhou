
#include "creature.h"
#include "MapLayer.h"
#include "GameResource.h"
#include "logging.h"

USING_NS_CC;

Creature::Creature()
    : mSprite(NULL)
    , mHpBar(new HpBar())
    , mMapLayer(NULL)
    , mTarget(NULL)
    , mAI(NULL)
    , mMaxSpeed(5)
    , mSpeed(5)
    , mLevel(1)
    , mExp(0)
    , mProjectileHelper(this)
{
    mCurrentHP = maxHp();
    mHpBar->getBar()->setTag(GameResource::nextTag());
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
    mSprite->setTag(GameResource::nextTag());
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char* file)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file);
    mSprite->setTag(GameResource::nextTag());
    return mSprite;
}

cocos2d::CCSprite* Creature::createSprite(const char * file, cocos2d::CCRect& rect)
{
    mSprite = new CCSprite();
    mSprite->initWithFile(file, rect);
    mSprite->setTag(GameResource::nextTag());
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


bool Creature::attackAnimate(Creature& o, MapLayer* gml)
{
    CCPoint hpt = getSprite()->getPosition();
    CCPoint bpt = getBar()->getPosition();
    CCPoint opt = o.getSprite()->getPosition();

    CCPoint dp((opt.x - hpt.x) / 2.0f, (opt.y - hpt.y) / 2.0f);

    CCPoint pt = ccpAdd(hpt, dp);
    CCMoveTo* cth1 = CCMoveTo::create(MELEEDURATION, pt);
    CCMoveTo* cth2 = CCMoveTo::create(MELEEDURATION, hpt);
    CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(Creature::onAttackFinished));
    getSprite()->runAction(CCSequence::create(cth1, cth2, cf, NULL));

    pt = ccpAdd(bpt, dp);
    CCMoveTo* ctb1 = CCMoveTo::create(MELEEDURATION, pt);
    CCMoveTo* ctb2 = CCMoveTo::create(MELEEDURATION, bpt);
    getBar()->runAction(CCSequence::create(ctb1, ctb2, NULL));
    return true;
}

void Creature::onAttackFinished(CCObject* pSender)
{
    // GameMap* gamemap = GameResource::getInstance()->gameMap();
    if (mTarget != NULL) {
        mTarget->mCurrentHP -= 3;
        if (mTarget->mCurrentHP <= 0)
            mTarget->mCurrentHP = 0;
        mTarget->mHpBar->setPercent((float)mTarget->currentHp() / mTarget->maxHp());
    }
    mMapLayer->onAttackFinished(this, mTarget);
    mTarget = NULL;
}

bool Creature::attack(Creature& o, MapLayer* gml)
{
    mMapLayer = gml;
    mTarget = &o;

    if (!attackAnimate(o, gml))
        return false;
    return true;
}

bool Creature::attack(Creature* o, MapLayer* gml)
{
    if (o)
        return attack(*o, gml);
    return false;
}

int Creature::sight()
{
    return 12;
}

int Creature::senseRange()
{
    return 20;
}

void Creature::onTurn(MapLayer* gml)
{
    if (mAI)
        mAI->turn(gml);
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

bool Creature::move(int dx, int dy, GameMap* mp)
{
    StartWalkingAnimation(dx, dy);

    CCPoint pt = getSprite()->getPosition();
    pt.x += dx * TILESIZE;
    pt.y += dy * TILESIZE;
    CCMoveTo* cth = CCMoveTo::create(HEROMOVEPERIOD, pt);
    getSprite()->runAction(cth);

    pt = getBar()->getPosition();
    pt.x += dx * TILESIZE;
    pt.y += dy * TILESIZE;
    CCMoveTo* ctb = CCMoveTo::create(HEROMOVEPERIOD, pt);
    getBar()->runAction(ctb);

    MoveTo(x + dx, y + dy, mp);
    return true;
}

bool Creature::findPathAndMove(int dx, int dy)
{
    GameMap* gamemap = GameResource::getInstance()->gameMap();
    FpHelper    ch; 
    if (!gamemap->findPathTo(x, y, dx, dy, ch)) {
        return false;
    }   
    ch.nodes.pop_front();
    CCPoint direction = ccpSub(ch.nodes.front(), ccp(x, y));
    if (direction.x == 0 && direction.y == 0) {
        FATAL() << "cannot runhere.";
        return false;
    }   

    move(direction.x, direction.y, gamemap);
    return true;
}

bool Creature::fireBall(Creature&o, MapLayer* gml)
{
    CCParticleMeteor* pm = CCParticleMeteor::create();
    pm->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fireball.png"));
    pm->setScale(0.2f);

    mProjectileHelper.set(this);
    float dist = GameResource::getInstance()->gameMap()->calcDistance(x, y, o.x, o.y);
    if (!gml->wrapper()->addProjectile(pm, ccp(x, y), ccp(o.x, o.y), dist * PROJECTILESPEED, &mProjectileHelper))
        return false;
    return true;
}

