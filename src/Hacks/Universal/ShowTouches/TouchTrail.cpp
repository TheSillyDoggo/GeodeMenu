#include "TouchTrail.hpp"
#include "Colours.hpp"
#include <Speedhack.hpp>

bool CCTouchTrail::init(CCTouch* touch)
{
    if (!CCNode::init())
        return false;

    touches.emplace(touch, this);
    
    this->touch = touch;
    this->scheduleUpdate();

    streak = CCMotionStreak::create(0.3f, 2, 10, ccc3(255, 255, 255), "streak_05_001.png");
    streak->setUserObject("ignore-longer-trail"_spr, CCBool::create(true));
    streak->unscheduleUpdate();

    circle = CCSprite::createWithSpriteFrameName("d_circle_02_001.png");    
    circle->setOpacity(150);

    auto clip = CCClippingNode::create(circle);
    clip->setAlphaThreshold(0.03f);
    clip->setInverted(true);

    clip->addChild(streak);

    update(0);

    this->addChild(clip);
    this->addChild(circle);
    return true;
}

void CCTouchTrail::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();

    auto pos = touch->getLocation();

    streak->setPosition(pos);
    circle->setPosition(pos);

    circle->setColor(CircleColour::get()->getColour());
    streak->setColor(TrailColour::get()->getColour());

    circle->setScale(TrailScale::get()->getStringFloat() * 0.2f);
    streak->setStroke(TrailScale::get()->getStringFloat() * 10);

    streak->CCMotionStreak::update(dt);
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