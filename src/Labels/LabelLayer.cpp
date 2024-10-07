#include "LabelLayer.hpp"
#include "../Client/Windows/Labels.hpp"

UILayer* uiLayer;

#define ANCHOR_CREATE(__anchor) nodes.emplace(LabelAnchor::__anchor, createNodeForAnchor(LabelAnchor::__anchor))

bool LabelLayer::init(UILayer* uiLayer)
{
    if (!CCLayer::init())
        return false;

    this->uiLayer = uiLayer;
    instance = this;

    this->scheduleUpdate();

    ANCHOR_CREATE(TopLeft);
    ANCHOR_CREATE(TopCenter);
    ANCHOR_CREATE(TopRight);
    ANCHOR_CREATE(CenterRight);
    ANCHOR_CREATE(BottomRight);
    ANCHOR_CREATE(BottomCenter);
    ANCHOR_CREATE(BottomLeft);
    ANCHOR_CREATE(CenterLeft);
    ANCHOR_CREATE(Center);

    for (auto node : nodes)
        this->addChild(node.second);

    for (auto mod : Labels::get()->modules)
    {
        if (auto mod2 = typeinfo_cast<LabelModule*>(mod))
        {
            auto node = LabelNode::create(mod2);

            labels.push_back(node);
        }
    }

    updateAnchors();

    return true;
}

void LabelLayer::update(float dt)
{
    _timeLeft -= dt / CCScheduler::get()->getTimeScale();
    _accum += 1 / (dt / CCScheduler::get()->getTimeScale());
    _frames++;

    if (_timeLeft <= 0)
    {
        fps = _accum / _frames;

        _timeLeft = _updateInterval;
        _accum = 0;
        _frames = 0;
    }

    for (auto label : labels)
    {
        label->update(dt);
    }

    for (auto node : nodes)
    {
        node.second->updateLayout();
    }

    for (auto label : labels)
    {
        label->setPosition(label->getPosition() + label->mod->offset);
    }
}

void LabelLayer::updateAnchors()
{
    for (auto node : labels)
    {
        node->retain();
    }

    for (auto node : nodes)
        node.second->removeAllChildrenWithCleanup(false);

    for (auto node : labels)
    {
        nodeForAnchor(node->mod->getSide())->addChild(node);

        node->release();
    }
}

void LabelLayer::incrementAttempts()
{
    attempts++;
}

int LabelLayer::getAttempts()
{
    return attempts;
}

float LabelLayer::getFPS()
{
    return fps;
}

int LabelLayer::getCPS()
{
    return 0;
}

CCNode* LabelLayer::nodeForAnchor(LabelAnchor anchor)
{
    return nodes.at(anchor);
}

CCNode* LabelLayer::createNodeForAnchor(LabelAnchor anchor)
{
    auto node = CCNode::create();
    node->setContentSize(CCDirector::get()->getWinSize());

    CCPoint point = CCPointZero;

    if (anchor == LabelAnchor::TopLeft     ) point = ccp(0, 1);
    if (anchor == LabelAnchor::TopCenter   ) point = ccp(0.5f, 1);
    if (anchor == LabelAnchor::TopRight    ) point = ccp(1, 1);
    if (anchor == LabelAnchor::CenterRight ) point = ccp(1, 0.5f);
    if (anchor == LabelAnchor::BottomRight ) point = ccp(1, 0);
    if (anchor == LabelAnchor::BottomCenter) point = ccp(0.5f, 0);
    if (anchor == LabelAnchor::CenterLeft  ) point = ccp(0, 0.5f);
    if (anchor == LabelAnchor::Center      ) point = ccp(0.5f, 0.5f);
    
    node->setAnchorPoint(point);
    node->setPosition(CCDirector::get()->getWinSize() * point);

    auto layout = AxisLayout::create(Axis::Column);
    layout->ignoreInvisibleChildren(true);
    layout->setAutoScale(false);
    layout->setGap(0);

    if (point.y != 0)
        layout->setAxisReverse(true);

    layout->setAxisAlignment(point.y == 0 ? AxisAlignment::Start : point.y == 0.5f ? AxisAlignment::Center : AxisAlignment::End);
    layout->setCrossAxisAlignment(point.x == 0 ? AxisAlignment::Start : point.x == 0.5f ? AxisAlignment::Center : AxisAlignment::End);
    layout->setCrossAxisLineAlignment(layout->getCrossAxisAlignment());

    node->setLayout(layout);
    return node;
}

LabelLayer* LabelLayer::create(UILayer* uiLayer)
{
    auto pRet = new LabelLayer();

    if (pRet && pRet->init(uiLayer))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

LabelLayer* LabelLayer::get()
{
    return instance;
}

LabelLayer::~LabelLayer()
{
    instance = nullptr;
}