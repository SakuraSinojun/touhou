

#include "MapLayer.h"
#include "MapWrapper.h"
#include "GameResource.h"
#include "GameOverScene.h"
#include "creature.h"
#include "gamemap.h"
#include "ornaments/ornament.h"
#include "hero.h"
#include "logging.h"

#include <list>
#include <algorithm>

USING_NS_CC;

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
    mIsAttacking = false;

    this->setTouchEnabled(true);
    return true;
}

void MapLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void MapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if (mIsAttacking)
        return;

    CCTouch* touch = (CCTouch*)(touches->anyObject());
    Hero* hero = GameResource::getInstance()->hero();
    GameMap* gamemap = GameResource::getInstance()->gameMap();

    CCPoint pt = touch->getLocation();
    mDestPoint = pointToMap(pt);

    // TODO: touch self
    if (mDestPoint.x == hero->x && mDestPoint.y == hero->y)
        return;

    Creature* creature = gamemap->at(mDestPoint.x, mDestPoint.y)->creature;
    Ornament* ornament = gamemap->at(mDestPoint.x, mDestPoint.y)->ornament;
    float dist = gamemap->calcDistance(hero->x, hero->y, mDestPoint.x, mDestPoint.y);

    if (creature != NULL) {
        if (gamemap->calcDistance(creature->x, creature->y, hero->x, hero->y) <= hero->attackRange()) {
            if (!startAttacking(creature)) {
                onAttackFinished(hero, creature);
            }
        }
    } else if (ornament != NULL && dist < 2) {
        if (ornament->active())
            mMapWrapper->refresh();
    } else {
        mMapWrapper->mGridPosition = mDestPoint;
        mMapWrapper->showGrid(true);
        startMoving();
    }
}

bool MapLayer::startAttacking(Creature* c)
{
    if (c == NULL)
        return false;
    mIsAttacking = true;
    Hero* hero = GameResource::getInstance()->hero();
    if (!hero->attack(c, this))
        return false;
    // onTurn();
    return true;
}

void MapLayer::onAttackFinished(Creature* attacker, Creature* c)
{
    mIsAttacking = false;
    if (c == NULL)
        return;
    if (c->currentHp() <= 0) {
        onCreatureDie(c, attacker);
    }
    Hero* hero = GameResource::getInstance()->hero();
    if (hero == attacker) {
        onTurn();
    }
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
        if (gamemap->calcDistance(hero->x, hero->y, mDestPoint.x, mDestPoint.y) > 1.0f) {
            if (!gamemap->findPathTo(hero->x, hero->y, mDestPoint.x, mDestPoint.y, ch)) {
                ch.nodes.clear();
                if (!gamemap->findPath1Step(hero->x, hero->y, mDestPoint.x, mDestPoint.y, ch)) {
                    return false;
                }
            }
        } else {
            Creature* creature = gamemap->at(mDestPoint.x, mDestPoint.y)->creature;
            Ornament* ornament = gamemap->at(mDestPoint.x, mDestPoint.y)->ornament;
            if (creature != NULL)
                return false;
            if (ornament != NULL) {
                if (ornament->active()) {
                    mMapWrapper->refresh();
                }
                return false;
            } else if (gamemap->at(mDestPoint.x, mDestPoint.y)->canPass()) {
                ch.nodes.push_back(ccp(hero->x, hero->y));
                ch.nodes.push_back(mDestPoint);
            } else {
                return false;
            }
        }
        if (ch.nodes.empty())
            return false;
        ch.nodes.pop_front();
        if (ch.nodes.empty())
            return false;

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

    std::list<Creature*>::iterator it;
    for (it = mActiveCreatures.begin(); it != mActiveCreatures.end(); it++) {
        (*it)->getSprite()->stopAllActions();
        (*it)->getBar()->stopAllActions();
    }

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

void MapLayer::onCreatureDie(Creature* c, Creature* attacker)
{
    if (c == NULL)
        return;
    if (c == GameResource::getInstance()->hero()) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, GameOverScene::create()));
        return;
    }
    if (attacker) {
        attacker->addExp(c->expOnDeath());
    }
    mMapWrapper->removeChild(c->getSprite());
    mMapWrapper->removeChild(c->getBar());
    GameMap::Node* n = GameResource::getInstance()->gameMap()->at(c->x, c->y);
    n->creature = NULL;
    removeActiveCreature(c);
    delete c;
}





