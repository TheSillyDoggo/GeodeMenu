#include "TouchTrail.hpp"
#include "Colours.hpp"
#include <Speedhack.hpp>

using namespace geode::prelude;
using namespace qolmod;

bool TouchTrail::init()
{
    if (!CCNode::init())
        return false;

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

    this->addChild(clip);
    this->addChild(circle);
    this->addChild(durationLabel);
    return true;
}

void TouchTrail::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();
    duration += dt;

    streak->setPosition(position);
    circle->setPosition(position);
    durationLabel->setPosition(position);

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

void TouchTrail::setPosition(const CCPoint &position)
{
    this->position = position;
}