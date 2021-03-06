

#include "GameResource.h"
#include "hero.h"
#include "gamemap.h"

#include <stdlib.h>

static GameResource* m_resources = NULL;
int GameResource::mTag = 1000;

GameResource::GameResource()
    : mHero(NULL)
    , mGameMap(NULL)
    , mAction(CT_MOVE)
{
    srand(0);
    mTag = 1000;
    mHero = new Hero();
    mGameMap = new GameMap();
}

GameResource::~GameResource()
{
    if (mGameMap)
        delete mGameMap;
    if (mHero)
        delete mHero;
}

GameResource* GameResource::getInstance()
{
    if (m_resources == NULL)
        m_resources = new GameResource();
    return m_resources;
}

void GameResource::reset()
{
    if (m_resources)
        delete m_resources;
    m_resources = NULL;
}






