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

    durationLabel = CCLabelBMFont::create("", "bigFont.fnt");

    update(0);

    this->addChild(clip);
    this->addChild(circle);
    this->addChild(durationLabel);
    return true;
}

void CCTouchTrail::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();
    duration += dt;

    auto pos = touch->getLocation();

    streak->setPosition(pos);
    circle->setPosition(pos);
    durationLabel->setPosition(pos);

    circle->setColor(CircleColour::get()->getColour());
    streak->setColor(TrailColour::get()->getColour());

    circle->setScale(CircleScale::get()->getStringFloat() * 0.2f);
    streak->setStroke(CircleScale::get()->getStringFloat() * 10);
    durationLabel->setScale(CircleScale::get()->getStringFloat() * 0.3f);

    streak->setVisible(TrailEnabled::get()->getRealEnabled());
    durationLabel->setVisible(ShowTouchDuration::get()->getRealEnabled());

    streak->CCMotionStreak::update(dt);
    durationLabel->setString(fmt::format("{:.2f}", duration).c_str());
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