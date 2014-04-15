

#include "MapLayer.h"
#include "MapWrapper.h"
#include "GameResource.h"
#include "creature.h"
#include "gamemap.h"
#include "hero.h"
#include "logging.h"

#include <list>
#include <algorithm>

USING_NS_CC;

class FpHelper : public GameMap::FpCallbackFunctor {
public:
    virtual void operator() (int x, int y) {
        CCPoint pt(x, y);
        nodes.push_back(pt);
    }
    std::list<CCPoint> nodes;
};

bool MapLayer::init()
{
    if (!CCLayer::init())
        return false;

    // CCSize  visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    // CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    mMapWrapper = MapWrapper::create();
    mMapWrapper->setPosition(ccp(0, 0));
    // mMapWrapper->setContentSize(ccp(MAPWIDTH * TILESIZE, MAPHEIGHT * TILESIZE));
    // mMapWrapper->setAnchorPoint(ccp(0, 0));
    this->addChild(mMapWrapper);

    mIsMoving = false;

    this->setTouchEnabled(true);
    return true;
}

void MapLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void MapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint pt = touch->getLocation();
    mDestPoint = pointToMap(pt);
    mMapWrapper->mGridPosition = mDestPoint;
    mMapWrapper->showGrid(true);
    startMoving();
}

void MapLayer::startMoving()
{
    mHeroPath.clear();
    if (!mIsMoving) {
        if (moveBy1Grid()) {
            mIsMoving = true;
        }
    }
}

bool MapLayer::moveBy1Grid()
{
    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();

    if (mDestPoint.x != hero->x || mDestPoint.y != hero->y) {
        FpHelper    ch;
        if (!gamemap->findPath(hero->x, hero->y, mDestPoint.x, mDestPoint.y, ch)) {
            return false;
        }
        ch.nodes.pop_front();
        CCPoint direction = ccpSub(ch.nodes.front(), ccp(hero->x, hero->y));
        if (direction.x == 0 && direction.y == 0) {
            FATAL() << "cannot runhere.";
            return false;
        }

        // as a loop.
        std::list<cocos2d::CCPoint>::iterator it;
        for (it = mHeroPath.begin(); it != mHeroPath.end(); it++) {
            if ((*it).x == ch.nodes.front().x && (*it).y == ch.nodes.front().y)
                return false;
        }
        mHeroPath.push_back(ch.nodes.front());

        // MAP
        CCPoint pt = mMapWrapper->getPosition();
        pt.x -= direction.x * TILESIZE;
        pt.y -= direction.y * TILESIZE;
        CCMoveTo* ctm = CCMoveTo::create(MAPMOVEPERIOD, pt);
        CCCallFuncN* cf = CCCallFuncN::create(this, callfuncN_selector(MapLayer::onNextMoving));
        mMapWrapper->runAction(CCSequence::create(ctm, cf, NULL));

        // hero
        hero->move(direction.x, direction.y, gamemap);
        onTurn();

        return true;
    }
    return false;
}

void MapLayer::onNextMoving(cocos2d::CCObject* pSender)
{
    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();
    gamemap->centerMap(hero->x, hero->y);
    mMapWrapper->setPosition(ccp(0, 0));
    mMapWrapper->refresh();

    hero->StopWalkingAnimation();
    // mMapWrapper->stopAllActions();

    if (!moveBy1Grid()) {
        mIsMoving = false;
        mMapWrapper->showGrid(false);
        mMapWrapper->refresh();
    }
}

CCPoint MapLayer::pointToMap(CCPoint point)
{
    CCSize  visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCPoint pw = mMapWrapper->getPosition();
    point = ccpSub(point, pw);

    CCPoint center;
    center.x = origin.x + visibleSize.width / 2;
    center.y = origin.y + visibleSize.height / 2;

    CCPoint dd;
    dd.x = (MAPWIDTH % 2) / 2.0f;
    dd.y = (MAPHEIGHT % 2) / 2.0f;

    CCPoint dp;
    dp.x = floor((point.x - center.x) / TILESIZEF + dd.x);
    dp.y = floor((point.y - center.y) / TILESIZEF + dd.y);

    GameMap* gamemap = GameResource::getInstance()->gameMap();
    CCPoint pt;
    pt.x = gamemap->centerX + dp.x;
    pt.y = gamemap->centerY + dp.y;

    return pt;
}

void MapLayer::onTurn()
{
    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();

    for (int x = hero->x - hero->senseRange(); x < hero->x + hero->senseRange(); x++) {
        for (int y = hero->y - hero->senseRange(); y < hero->y + hero->senseRange(); y++) {
            Creature* c = gamemap->at(x, y)->creature;
            if (c != NULL && c != hero)
                addActiveCreature(c);
        }
    }

    if (mActiveCreatures.empty())
        return;

    std::list<Creature*>::iterator it = mActiveCreatures.begin();

    while (it != mActiveCreatures.end()) {
        Creature* c = *it;
        float dist = gamemap->calcDistance(c->x, c->y, hero->x, hero->y);
        if (dist > c->senseRange() || dist > c->sight()) {
            it = mActiveCreatures.erase(it);
        } else {
            c->onTurn(this);
            it++;
        }
    }
}

void MapLayer::removeActiveCreature(Creature* c)
{
    if (c == NULL)
        return;
    std::list<Creature*>::iterator it;
    it = std::find(mActiveCreatures.begin(), mActiveCreatures.end(), c);
    if (it != mActiveCreatures.end())
        mActiveCreatures.erase(it);
}

void MapLayer::addActiveCreature(Creature* c)
{
    if (c == NULL)
        return;
    std::list<Creature*>::iterator it;
    it = std::find(mActiveCreatures.begin(), mActiveCreatures.end(), c);
    if (it == mActiveCreatures.end())
        mActiveCreatures.push_back(c);
}

bool MapLayer::moveCreature(Creature* c, cocos2d::CCPoint dest)
{
    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();

    FpHelper    ch;
    if (!gamemap->findPathTo(c->x, c->y, hero->x, hero->y, ch)) {
        return false;
    }
    ch.nodes.pop_front();
    CCPoint direction = ccpSub(ch.nodes.front(), ccp(c->x, c->y));
    if (direction.x == 0 && direction.y == 0) {
        FATAL() << "cannot runhere.";
        return false;
    }

    c->move(direction.x, direction.y, gamemap);
    return true;
}





