#include "BetterRotationControl.hpp"
#include "../Utils/Num.hpp"

void BetterRotationControl::setAngle(float angle)
{
    float angleRadians = CC_DEGREES_TO_RADIANS(angle);

    float dx = std::cos(angleRadians);
    float dy = std::sin(angleRadians);
    auto point = ccp(dx, dy);

    handle->setPosition(point * 60);
}

void BetterRotationControl::setDelegate(GJRotationControlDelegate* delegate)
{
    this->delegate = delegate;
}

bool BetterRotationControl::init()
{
    if (!CCMenu::init())
        return false;

    this->setContentSize(ccp(0, 0));
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setTouchPriority(-80085);
    this->setTouchEnabled(true);

    handle = CCSprite::createWithSpriteFrameName("GJ_rotationControlBtn01_001.png");

    setAngle(0);

    this->addChild(handle);
    return true;
}

void BetterRotationControl::draw()
{
    cocos2d::ccDrawColor4B(255, 255, 255, 50);
    glLineWidth(1.0f);
    cocos2d::ccDrawCircle(ccp(0, 0), 60.0f, 0, 0x50, false);
}

bool BetterRotationControl::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    auto bb = getWorldSpaceBoundingBox(handle);
    auto point = ccp(bb.getMidX(), bb.getMidY());

    if (cocos2d::ccpDistance(point, touch->getLocation()) < 20 * getScale())
    {
        handle->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("GJ_rotationControlBtn02_001.png"));

        return true;
    }

    return false;
}

void BetterRotationControl::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    auto bb = getWorldSpaceBoundingBox(this);
    auto point = ccp(bb.getMidX(), bb.getMidY());

    auto angle = CC_RADIANS_TO_DEGREES(ccpToAngle(touch->getLocation() - point));

    setAngle(angle);

    if ((int)angle != lastAngle)
    {
        if (delegate)
            delegate->angleChanged((int)angle);

        lastAngle = (int)angle;
    }
}

void BetterRotationControl::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    handle->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("GJ_rotationControlBtn01_001.png"));
}