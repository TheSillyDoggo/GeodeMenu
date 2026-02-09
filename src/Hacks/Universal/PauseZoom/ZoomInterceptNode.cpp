#include "ZoomInterceptNode.hpp"

bool ZoomInterceptNode::init()
{
    if (!CCLayer::init())
        return false;

    this->setTouchEnabled(true);
    this->setTouchPriority(-10000);
    this->setMouseEnabled(true);
    this->scheduleUpdate();
    this->registerWithTouchDispatcher();

    setPreviewPos(getContentSize() / 2);
    return true;
}

void ZoomInterceptNode::update(float dt)
{
    if (auto pl = PlayLayer::get())
    {
        pl->setScale(1.0f + zoom);
        pl->setPosition((-zoomedPos + (pl->getContentSize() * pl->getAnchorPoint())) * pl->getScale());
    }
}

ZoomInterceptNode::~ZoomInterceptNode()
{
    if (auto pl = PlayLayer::get())
    {
        pl->setScale(1);
        pl->setPosition(ccp(0, 0));
    }
}

void ZoomInterceptNode::scrollWheel(float y, float x)
{
    float oldZoom = zoom;

    setZoom(zoom + (y * -0.03f));

    if (zoom == 0)
    {
        return;
    }

    CCPoint endPos;

    if (y < 0)
        endPos = getMousePos();
    else
        endPos = getContentSize() / 2;

    auto abs = std::abs<float>(zoom - oldZoom);

    // log::info("abs: {}", abs);

    setPreviewPos(ccpLerp(zoomedPos, endPos, abs));
}

bool ZoomInterceptNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
    log::info("began: {}", pTouch->getLocation());

    return true;
}

void ZoomInterceptNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void ZoomInterceptNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void ZoomInterceptNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}

void ZoomInterceptNode::setPreviewPos(CCPoint pos)
{
    this->zoomedPos = pos;
}

void ZoomInterceptNode::setZoom(float zool)
{
    zoom = std::clamp<float>(zool, 0, 5);

    if (zoom == 0)
        setPreviewPos(getContentSize() / 2);
    else
        updatePositions();
}

void ZoomInterceptNode::updatePositions()
{

}