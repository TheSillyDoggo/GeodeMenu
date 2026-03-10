#include "ZoomInterceptNode.hpp"
#include "ZoomPause.hpp"

ZoomInterceptNode* ZoomInterceptNode::create(CCLayer* layer)
{
    auto pRet = new ZoomInterceptNode();
    pRet->layer = layer;

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

ZoomInterceptNode* ZoomInterceptNode::get()
{
    return instance;
}

bool ZoomInterceptNode::init()
{
    if (!CCLayer::init())
        return false;

    instance = this;
    // CCTouchDispatcher::get()->registerForcePrio(this, 2);

    this->setTouchEnabled(true);
    this->setMouseEnabled(true);
    this->scheduleUpdate();
    this->registerWithTouchDispatcher();

    auto control = qolmod::ZoomControl::create();
    CCScene::get()->addChild(control, CCScene::get()->getHighestChildZ() + 1);

    setPreviewPos(getContentSize() / 2);
    return true;
}

void ZoomInterceptNode::update(float dt)
{
    if (layer)
    {
        layer->setScale(1.0f + zoom);
        layer->setPosition(zoomedPos - getContentSize() / 2);

        if (!isZooming())
        {
            layer->setScale(1);
            layer->setPosition(ccp(0, 0));
        }
    }
}

ZoomInterceptNode::~ZoomInterceptNode()
{
    if (layer)
    {
        layer->setScale(1);
        layer->setPosition(ccp(0, 0));
    }

    if (instance == this)
        instance = nullptr;
    
    // CCTouchDispatcher::get()->unregisterForcePrio(this);
    CCTouchDispatcher::get()->removeDelegate(this);
}

void ZoomInterceptNode::scrollWheel(float y, float x)
{
    if (!ZoomWithScroll::get()->getRealEnabled())
        return;

    float oldZoom = zoom;
    float oldScale = 1.0f + oldZoom;

    setZoom(zoom + (y * -0.03f));

    if (zoom == oldZoom)
        return;

    float newScale = 1.0f + zoom;

    CCPoint mouse = getMousePos();
    CCPoint oldPos = zoomedPos;

    float scaleRatio = newScale / oldScale;

    CCPoint newPos;
    newPos.x = mouse.x - (mouse.x - oldPos.x) * scaleRatio;
    newPos.y = mouse.y - (mouse.y - oldPos.y) * scaleRatio;

    setPreviewPos(newPos);
}

void ZoomInterceptNode::registerWithTouchDispatcher()
{
    CCTouchDispatcher::get()->addTargetedDelegate(this, -503, false);
}

bool ZoomInterceptNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (touch1 && touch2)
        return false;

    if (!touch1)
    {
        if (isZooming())
            isPanning = true;

        touch1 = pTouch;
    }
    else
    {
        touch2 = pTouch;

        lastPinchDis = ccpDistance(touch1->getLocation(), touch2->getLocation());
    }

    return true;
}

void ZoomInterceptNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (touch1 && touch2 && ZoomWithTouch::get()->getRealEnabled())
    {
        CCPoint p1 = touch1->getLocation();
        CCPoint p2 = touch2->getLocation();

        float newDistance = ccpDistance(p1, p2);
        float delta = newDistance - lastPinchDis;

        float ratio = newDistance / lastPinchDis;

        lastPinchDis = newDistance;

        float oldZoom = zoom;
        float oldScale = 1.0f + oldZoom;

        float newScale2 = oldScale * ratio;

        setZoom(newScale2 - 1.0f);

        if (zoom == oldZoom)
            return;

        float newScale = 1.0f + zoom;

        CCPoint center = ccpMidpoint(p1, p2);
        CCPoint oldPos = zoomedPos;

        float scaleRatio = newScale / oldScale;

        CCPoint newPos;
        newPos.x = center.x - (center.x - oldPos.x) * scaleRatio;
        newPos.y = center.y - (center.y - oldPos.y) * scaleRatio;

        setPreviewPos(newPos);
    }

    if (isPanning && ZoomPanning::get()->getRealEnabled())
    {
        setPreviewPos(zoomedPos + pTouch->getDelta());
    }
}

void ZoomInterceptNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (pTouch == touch1)
        touch1 = nullptr;

    if (pTouch == touch2)
        touch2 = nullptr;
}

void ZoomInterceptNode::setPreviewPos(CCPoint pos)
{
    this->zoomedPos = pos;

    updatePositions();
}

void ZoomInterceptNode::setZoom(float zool)
{
    zoom = std::clamp<float>(zool, 0, NoZoomLimit::get()->getRealEnabled() ? zool : 9);

    if (zoom == 0)
        setPreviewPos(getContentSize() / 2);

    updatePositions();
}

void ZoomInterceptNode::updatePositions()
{
    if (!layer)
        return;

    auto win = CCDirector::sharedDirector()->getWinSize();
    auto size = layer->getContentSize();
    auto scale = 1 + zoom;

    auto half = ccp(size.width * scale / 2.f, size.height * scale / 2.f);

    // boy love
    auto bl = zoomedPos - half;
    auto tr = zoomedPos + half;

    if (bl.x > 0.f)
        zoomedPos.x -= bl.x;

    if (bl.y > 0.f)
        zoomedPos.y -= bl.y;

    if (tr.x < win.width)
        zoomedPos.x += win.width - tr.x;

    if (tr.y < win.height)
        zoomedPos.y += win.height - tr.y;
}

bool ZoomInterceptNode::isZooming()
{
    if (!ZoomPauseMenu::get()->getRealEnabled())
        return false;

    return zoom != 0;
}

float ZoomInterceptNode::getZoom()
{
    return zoom;
}

CCPoint ZoomInterceptNode::getPreviewPos()
{
    return zoomedPos;
}