#include "GrabNodeLayer.hpp"
#include <Utils.hpp>

bool GrabNodeLayer::init()
{
    if (!CCLayer::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setPosition(CCPointZero);
    this->setContentSize(CCPointZero);
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    this->setTouchEnabled(true);

    return true;
}

CCPoint GrabNodeLayer::getAxisForLocked(LockedAxis axis)
{
    switch (axis)
    {
        case LockedAxis::Horizontal:
            return ccp(1, 0);

        case LockedAxis::Vertical:
            return ccp(0, 1);

        default:
            return ccp(1, 1);
    }
}

void GrabNodeLayer::setLockedAxis(LockedAxis axis)
{
    this->axis = axis;
}

LockedAxis GrabNodeLayer::getLockedAxis()
{
    return axis;
}

void GrabNodeLayer::setNodeToGrab(CCNode* node)
{
    nodeToGrab = node;
}

CCNode* GrabNodeLayer::getNodeToGrab()
{
    return nodeToGrab;
}

void GrabNodeLayer::setNodeContainerThing(CCNode* node)
{
    this->nodeContainerThing = node;
}

CCNode* GrabNodeLayer::getNodeContainerThing()
{
    return nodeContainerThing;
}

void GrabNodeLayer::setOnStartDrag(std::function<void()> callback)
{
    this->onStartDrag = callback;
}

void GrabNodeLayer::setOnMoveDrag(std::function<void()> callback)
{
    this->onMoveDrag = callback;
}

void GrabNodeLayer::setOnEndDrag(std::function<void()> callback)
{
    this->onEndDrag = callback;
}

void GrabNodeLayer::registerWithTouchDispatcher(void)
{
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}

bool GrabNodeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (nodeToGrab && cocos::nodeIsVisible(this))
    {
        auto r = qolmod::utils::getBasicRect(this);
        r.origin = CCPointZero;

        if (r.containsPoint(convertToNodeSpace(pTouch->getLocation())))
        {
            isDragging = true;
            startPos = nodeToGrab->getPosition();

            if (onStartDrag)
                onStartDrag();

            return true;
        }
    }

    return false;
}

void GrabNodeLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isDragging)
    {
        if (nodeContainerThing)
            nodeToGrab->setPosition((nodeContainerThing->convertToNodeSpace(pTouch->getLocation()) / CCDirector::get()->getWinSize() * nodeContainerThing->getContentSize()) * getAxisForLocked(axis));
        else
            nodeToGrab->setPosition(startPos + (pTouch->getLocation() - pTouch->getStartLocation()) * getAxisForLocked(axis));

        if (onMoveDrag)
            onMoveDrag();
    }
}

void GrabNodeLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    isDragging = false;

    if (onEndDrag)
        onEndDrag();
}

void GrabNodeLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}

bool GrabNodeLayer::getDragging()
{
    return isDragging;
}