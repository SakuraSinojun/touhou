

#include "GameResource.h"
#include "hero.h"
#include "gamemap.h"

#include <stdlib.h>

static GameResource* m_resources = NULL;

GameResource::GameResource()
    : mHero(NULL)
    , mGameMap(NULL)
    , mAction(CT_MOVE)
{
    srand(0);
    mHero = new Hero();
    mGameMap = new GameMap();
}

GameResource::~GameResource()
{
    if (mHero)
        delete mHero;
    if (mGameMap)
        delete mGameMap;
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






