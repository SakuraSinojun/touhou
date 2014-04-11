

#include "HpBar.h"

USING_NS_CC;

HpBar::HpBar()
    : mBackground(NULL)
    , mBar(NULL)
{
    mBackground = new CCSprite();
    mBackground->initWithFile("particle/hpbarbg.png");
    mBar = new CCSprite();
    mBar->initWithFile("particle/hpbar.png");
    mBackground->addChild(mBar);

    CCPoint pt;
    pt.x = 0;
    pt.y = 0.5;
    mBar->setAnchorPoint(pt);
    pt.x = 0;
    pt.y = mBackground->getContentSize().height / 2;
    mBar->setPosition(pt);

    mBackground->setScale(0.8);
}

HpBar::~HpBar()
{
    if (mBackground)
        delete mBackground;
    if (mBar)
        delete mBar;
}
    
void HpBar::setPercent(float per)
{
    mBar->setScaleX(per);
}




