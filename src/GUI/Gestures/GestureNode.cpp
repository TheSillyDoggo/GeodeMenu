#include "GestureNode.hpp"
#include <Speedhack.hpp>
#include <Utils.hpp>

using namespace geode::prelude;
using namespace qolmod;

bool GestureNode::init()
{
    if (!CCNodeRGBA::init())
        return false;

    setAnchorPoint(ccp(0, 0.5f));

    cornerTop = UserSprite::create(fmt::format("<sprite>{}gesture-corner.png", ""_spr));
    cornerBottom = UserSprite::create(fmt::format("<sprite>{}gesture-corner.png", ""_spr));

    cornerTop->setAnchorPoint(ccp(0, 0));
    cornerBottom->setAnchorPoint(ccp(0, 0));
    cornerBottom->setRotation(90);

    sideFill = CCLayerColor::create(ccc4(255, 255, 255, 255));
    sideFill->ignoreAnchorPointForPosition(false);
    sideFill->setAnchorPoint(ccp(0, 0.5f));
    sideFill->setPosition(CCPointZero);

    gapFill = CCLayerColor::create(ccc4(255, 255, 255, 255));
    gapFill->ignoreAnchorPointForPosition(false);
    gapFill->setAnchorPoint(ccp(0, 0.5f));
    gapFill->setPosition(CCPointZero);

    anchor = GestureAnchor::Left;
    realPosition = ccp(0, CCDirector::get()->getWinSize().height * 0.75f);

    scheduleUpdate();
    setContentSize(ccp(4, 50));
    setOpacity(100);

    this->addChild(cornerTop);
    this->addChild(cornerBottom);
    this->addChild(sideFill);
    this->addChild(gapFill);
    return true;
}

void GestureNode::update(float dt)
{
    dt = Speedhack::get()->getRealDeltaTime();
    auto winSize = CCDirector::get()->getWinSize();

    switch (anchor)
    {
        case GestureAnchor::Left:
            setRotation(0);
            setPosition(ccp(0, displayedPosition.y));
            break;

        case GestureAnchor::Top:
            setRotation(90);
            setPosition(ccp(displayedPosition.x, winSize.height));
            break;

        case GestureAnchor::Right:
            setRotation(180);
            setPosition(ccp(winSize.width, displayedPosition.y));
            break;

        case GestureAnchor::Bottom:
            setRotation(270);
            setPosition(ccp(displayedPosition.x, 0));
            break;
    }

    float t = 10 * Speedhack::get()->getRealDeltaTime();

    displayedPosition = ccp(
        std::lerp<double>(displayedPosition.x, realPosition.x, t),
        std::lerp<double>(displayedPosition.y, realPosition.y, t)
    );
}

bool GestureNode::touchBegan(qolmod::Touch* touch)
{
    auto local = convertToNodeSpace(touch->location);
    log::info("local: {}", local);
    
    if (CCRectMake(0, 0, getContentWidth(), getContentHeight()).containsPoint(local))
    {
        return true;
    }

    return false;
}

void GestureNode::touchMoved(qolmod::Touch* touch)
{
    realPosition = touch->location;
}

void GestureNode::touchEnded(qolmod::Touch* touch)
{

}

void GestureNode::touchCancelled(qolmod::Touch* touch)
{
    touchEnded(touch);
}

void GestureNode::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);
    
    gapFill->setContentSize(ccp(contentSize.width - cornerTop->getContentWidth(), contentSize.height));
    gapFill->setPositionY(contentSize.height / 2);

    sideFill->setContentSize(ccp(cornerTop->getContentWidth(), contentSize.height - cornerTop->getContentHeight() * 2));
    sideFill->setPosition(ccp(contentSize.width - cornerTop->getContentWidth(), contentSize.height / 2));

    cornerTop->setPosition(contentSize.width - cornerTop->getContentWidth(), contentSize.height - cornerTop->getContentHeight());
    cornerBottom->setPosition(contentSize.width - cornerTop->getContentWidth(), cornerBottom->getContentHeight());
}

void GestureNode::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);

    cornerTop->setOpacity(opacity);
    cornerBottom->setOpacity(opacity);
    sideFill->setOpacity(opacity);
    gapFill->setOpacity(opacity);
}

void GestureNode::setColor(const ccColor3B& color)
{
    CCNodeRGBA::setColor(color);

    cornerTop->setColor(color);
    cornerBottom->setColor(color);
    sideFill->setColor(color);
    gapFill->setColor(color);
}

void GestureNode::setAnchor(qolmod::GestureAnchor anchor)
{
    if (this->anchor == anchor)
        return;

    auto winSize = CCDirector::get()->getWinSize();
    auto prev = this->anchor;
    this->anchor = anchor;

    switch (prev)
    {
        case GestureAnchor::Top:
            displayedPosition.y = winSize.height;
            break;

        case GestureAnchor::Bottom:
            displayedPosition.y = 0;
            break;

        case GestureAnchor::Left:
            displayedPosition.y = 0;
            break;

        case GestureAnchor::Right:
            displayedPosition.y = winSize.width;
            break;
    }
}