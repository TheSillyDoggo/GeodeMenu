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

    return true;
}

void ZoomInterceptNode::update(float dt)
{
    return;

    if (auto pl = PlayLayer::get())
    {
        pl->setScale(1.0f + zoom);
        pl->setPosition(zoomedPos);
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
    return;

    // if (y < 0)
        zoom += y * -0.03f;
    // else

    zoomedPos = ccpLerp(zoomedPos, (getMousePos() - (CCDirector::get()->getWinSize() / 2)), (6 * 0.03f));

    zoom = std::clamp<float>(zoom, 0, 5);
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