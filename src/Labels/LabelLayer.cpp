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

    updateLabels();

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

    for (size_t i = 0; i < cps1.size(); i++)
        cps1[i] -= dt;

    for (size_t i = 0; i < cps2.size(); i++)
        cps2[i] -= dt;

    cps1.erase(std::remove_if(cps1.begin(), cps1.end(), [](float i){ return i < 0; }), cps1.end());
    cps2.erase(std::remove_if(cps2.begin(), cps2.end(), [](float i){ return i < 0; }), cps2.end());   

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

    nodes.at(LabelAnchor::BottomLeft)->setPosition(Labels::get()->safeZone.origin);
    nodes.at(LabelAnchor::CenterLeft)->setPositionX(Labels::get()->safeZone.origin.x);
    nodes.at(LabelAnchor::TopLeft)->setPositionX(Labels::get()->safeZone.origin.x);
    nodes.at(LabelAnchor::TopLeft)->setPositionY(CCDirector::get()->getWinSize().height - Labels::get()->safeZone.size.height);
    nodes.at(LabelAnchor::BottomCenter)->setPositionY(Labels::get()->safeZone.origin.y);
    nodes.at(LabelAnchor::BottomRight)->setPositionY(Labels::get()->safeZone.origin.y);
    nodes.at(LabelAnchor::BottomRight)->setPositionX(CCDirector::get()->getWinSize().width - Labels::get()->safeZone.size.width);
    nodes.at(LabelAnchor::CenterRight)->setPositionX(CCDirector::get()->getWinSize().width - Labels::get()->safeZone.size.width);
    nodes.at(LabelAnchor::TopRight)->setPositionX(CCDirector::get()->getWinSize().width - Labels::get()->safeZone.size.width);
    nodes.at(LabelAnchor::TopRight)->setPositionY(CCDirector::get()->getWinSize().height - Labels::get()->safeZone.size.height);
    nodes.at(LabelAnchor::TopCenter)->setPositionY(CCDirector::get()->getWinSize().height - Labels::get()->safeZone.size.height);
}

void LabelLayer::triggerEvent(LabelEventType type)
{
    for (auto module : Labels::get()->modules)
    {
        if (auto mod = typeinfo_cast<LabelModule*>(module))
        {
            for (auto event : mod->events)
            {
                if (event.type == type)
                {
                    auto array = CCArray::create();
                    array->retain();

                    if (event.fadeIn != -1)
                        array->addObject(CCTintTo::create(event.fadeIn, event.colour.r, event.colour.g, event.colour.b));

                    if (event.hold != -1)
                        array->addObject(CCDelayTime::create(event.hold));

                    if (event.fadeOut != -1)
                        array->addObject(CCTintTo::create(event.fadeOut, mod->getColour().r, mod->getColour().g, mod->getColour().b));

                    auto seq = CCSequence::create(array);
                    mod->labelNode->runAction(seq);

                    array->release();
                }
            }
        }
    }
}

void LabelLayer::updateLabels()
{
    for (auto label : labels)
        label->removeFromParentAndCleanup(true);

    labels.clear();

    for (auto mod : Labels::get()->modules)
    {
        if (auto mod2 = typeinfo_cast<LabelModule*>(mod))
        {
            auto node = LabelNode::create(mod2);
            node->update(-1);

            labels.push_back(node);
        }
    }

    updateAnchors();
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

    for (auto node : nodes)
        node.second->updateLayout();
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

void LabelLayer::resetCPS()
{
    cps1.clear();
    cps2.clear();

    clicks1 = 0;
    clicks2 = 0;
}

void LabelLayer::increateCPS(bool player2)
{
    if (player2)
        cps2.push_back(1);
    else
        cps1.push_back(1);

    if (player2)
        clicks2++;
    else
        clicks1++;
}

int LabelLayer::getCPS(bool player2)
{
    return player2 ? cps2.size() : cps1.size();
}

int LabelLayer::getTotalCPS()
{
    return cps1.size() + cps2.size();
}

int LabelLayer::getClicks(bool player2)
{
    if (player2)
        return clicks2;
    else
        return clicks1;
}

int LabelLayer::getTotalClicks()
{
    return clicks1 + clicks2;
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