#include "LabelContainerLayer.hpp"
#include "TextLabelNode.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"

LabelContainerLayer* LabelContainerLayer::get()
{
    return instance;
}

LabelContainerLayer::~LabelContainerLayer()
{
    instance = nullptr;
}

bool LabelContainerLayer::init()
{
    if (!CCLayer::init())
        return false;

    instance = this;
    this->setTouchEnabled(false);
    this->scheduleUpdate();

    for (size_t i = 0; i < 9; i++)
    {
        addAnchorNode((LabelAnchor)i);
    }
    
    updateConfigs();
    return true;
}

void LabelContainerLayer::update(float dt)
{
    if (dt != -1)
    {
        dt = Speedhack::get()->getRealDeltaTime();

        for (size_t i = 0; i < totalCps.size(); i++)
            totalCps[i] += dt;

        for (size_t i = 0; i < p1Cps.size(); i++)
            p1Cps[i] += dt;

        for (size_t i = 0; i < p2Cps.size(); i++)
            p2Cps[i] += dt;

        totalCps.erase(std::remove_if(totalCps.begin(), totalCps.end(), [](float i){ return i > 1; }), totalCps.end());
        p1Cps.erase(std::remove_if(p1Cps.begin(), p1Cps.end(), [](float i){ return i > 1; }), p1Cps.end());
        p2Cps.erase(std::remove_if(p2Cps.begin(), p2Cps.end(), [](float i){ return i > 1; }), p2Cps.end());

        if (HideLabels::get()->getRealEnabled())
            return;
    }

    highestP1CPS = std::max<int>(p1Cps.size(), highestP1CPS);
    highestP2CPS = std::max<int>(p2Cps.size(), highestP2CPS);

    for (auto node : nodes)
    {
        node->updateGeneral(dt);
    }

    for (auto node : anchors)
    {
        sortNodeChildren(node.second);

        node.second->setPosition(getContentSize() * node.second->getAnchorPoint());
    }

    for (auto node : nodes)
    {
        node->setPosition(node->getPosition() + node->getLabelConfig().offset);
    }
}

void LabelContainerLayer::visit(void)
{
    if (HideLabels::get()->getRealEnabled())
        return;
    
    CCLayer::visit();
}

void LabelContainerLayer::sortNodeChildren(CCNode* node)
{
    float height = 0;
    float y = 0;

    for (size_t i = 0; i < node->getChildrenCount(); i++)
    {
        auto n = node->getChildByIndex<CCNode*>(i);

        if (!n->isVisible())
            continue;

        height += n->getScaledContentHeight();
    }
    
    for (size_t i = 0; i < node->getChildrenCount(); i++)
    {
        auto n = node->getChildByIndex<CCNode*>(i);

        if (!n->isVisible())
            continue;

        n->setPositionX(0);

        if (node->getAnchorPoint().y == 0)
            n->setPositionY(y - (height * node->getAnchorPoint().y));
        else if (node->getAnchorPoint().y == 0.5f)
            n->setPositionY((height / 2) - y);
        else
            n->setPositionY(-y);
        
        y += n->getScaledContentHeight();
    }
}

void LabelContainerLayer::addAnchorNode(LabelAnchor anchor)
{
    auto node = CCNode::create();
    anchors.emplace(anchor, node);
    node->setAnchorPoint(LabelManager::get()->anchorToPoint(anchor));
    node->setPosition(getContentSize() * LabelManager::get()->anchorToPoint(anchor));

    this->addChild(node);
}

void LabelContainerLayer::updateConfigs()
{
    auto safe = LabelManager::get()->getSafeZone();

    this->setContentSize(CCDirector::get()->getWinSize() - safe.origin - safe.size);
    this->setPosition(safe.origin);

    for (auto node : nodes)
    {
        node->removeFromParent();
    }
    
    nodes.clear();

    for (auto conf : LabelManager::get()->getConfigs())
    {
        auto node = LabelNode::createForType(conf.type);
        node->setLabelConfig(conf);
        node->setAnchorPoint(LabelManager::get()->anchorToPoint(conf.anchor));

        nodes.push_back(node);
        anchors[conf.anchor]->addChild(node);
    }

    update(-1);
}

int LabelContainerLayer::getCPS(NoclipPlayerSelector selector)
{
    switch (selector)
    {
        case NoclipPlayerSelector::All:
            return totalCps.size();
        
        case NoclipPlayerSelector::Player1:
            return p1Cps.size();
        
        case NoclipPlayerSelector::Player2:
            return p2Cps.size();
    }

    return 0;
}

int LabelContainerLayer::getTotalClicks(NoclipPlayerSelector selector)
{
    switch (selector)
    {
        case NoclipPlayerSelector::All:
            return totalClicks;
        
        case NoclipPlayerSelector::Player1:
            return p1Clicks;
        
        case NoclipPlayerSelector::Player2:
            return p2Clicks;
    }

    return 80085;
}

int LabelContainerLayer::getHighestCPS(NoclipPlayerSelector selector)
{
    switch (selector)
    {
        case NoclipPlayerSelector::All:
            return std::max<int>(highestP1CPS, highestP2CPS);
        
        case NoclipPlayerSelector::Player1:
            return highestP1CPS;
        
        case NoclipPlayerSelector::Player2:
            return highestP2CPS;
    }

    return 80085;
}

void LabelContainerLayer::onPlayerClicked(NoclipPlayerSelector selector)
{
    totalClicks++;
    totalCps.push_back(0);

    if (selector == NoclipPlayerSelector::Player1)
    {
        p1Clicks++;
        p1Cps.push_back(0);
    }
    else
    {
        p2Clicks++;
        p2Cps.push_back(0);
    }
}

void LabelContainerLayer::onNewAttempt()
{
    totalClicks = 0;
    p1Clicks = 0;
    p2Clicks = 0;
    highestP1CPS = 0;
    highestP2CPS = 0;
}

void LabelContainerLayer::onEventTriggered(LabelEventType type)
{
    for (auto node : nodes)
    {
        node->onEventTriggered(type);
    }
}