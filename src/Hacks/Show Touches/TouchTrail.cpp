#include "TouchTrail.hpp"

bool CCTouchTrail::init(CCTouch* touch)
{
    if (!CCNode::init())
        return false;

    touches.emplace(touch, this);
    
    this->touch = touch;
    this->scheduleUpdate();

    streak = CCMotionStreak::create(0.3f, 2, 10, ccc3(255, 255, 255), "streak_05_001.png");
    streak->setUserObject("ignore-longer-trail"_spr, CCBool::create(true));

    circle = CCSprite::createWithSpriteFrameName("d_circle_02_001.png");
    circle->setScale(0.2f);
    circle->setOpacity(150);

    auto clip = CCClippingNode::create(circle);
    clip->setAlphaThreshold(0.03f);
    clip->setInverted(true);

    clip->addChild(streak);

    this->addChild(clip);
    this->addChild(circle);
    return true;
}

void CCTouchTrail::update(float dt)
{
    auto pos = touch->getLocation();

    streak->setPosition(pos);
    circle->setPosition(pos);
}

CCTouchTrail* CCTouchTrail::create(CCTouch* touch)
{
    auto pRet = new CCTouchTrail();

    pRet->init(touch);

    return pRet;
}

void CCTouchTrail::remove(CCTouch* touch)
{
    if (!touches.contains(touch))
        return;

    auto t = touches[touch];
    t->removeFromParent();

    touches.erase(touch);

    delete t;
}