#include "CheatIndicator.hpp"
#include "../SafeMode.hpp"

using namespace geode::prelude;

bool CheatIndicator::init()
{
    if (!CCNode::init())
        return false;

    spr = CCSprite::createWithSpriteFrameName("uiDot_001.png");
    spr->setAnchorPoint(ccp(0, 0));

    update(0);

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentSize(spr->getContentSize());
    this->scheduleUpdate();
    this->addChild(spr);
    return true;
}

void CheatIndicator::update(float dt)
{
    spr->setColor(SafeMode::get()->getIndicatorColour());
}