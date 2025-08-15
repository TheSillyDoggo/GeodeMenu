#include "SimpleLabelNode.hpp"
#include "LabelUtils.hpp"

bool SimpleLabelNode::init()
{
    if (!CCLayer::init())
        return false;

    this->scheduleUpdate();
    this->setZOrder(420);

    auto zone = LabelUtils::get()->getSafeZone();
    setPosition(ccp(zone.origin.x, zone.size.height));
    setContentSize(ccp(getContentWidth() - zone.size.width - zone.origin.x, getContentHeight() - zone.size.height - zone.origin.y));

    for (size_t i = 0; i < 9; i++)
    {
        setupLabel(as<SimpleLabelAnchor>(i));
    }

    return true;
}

void SimpleLabelNode::setupLabel(SimpleLabelAnchor anchor)
{
    auto point = getPointForAnchor(anchor);

    auto lbl = CCLabelBMFont::create("", "bigFont.fnt");
    lbl->setAnchorPoint(point);
    lbl->setPosition(getContentSize() * point);

    lbl->setAlignment(point.x == 0.5f ? kCCTextAlignmentCenter : (point.x == 1 ? kCCTextAlignmentRight : kCCTextAlignmentLeft));
    
    this->addChild(lbl);
    labels.emplace(anchor, lbl);
    labelStrs.emplace(anchor, "");
}

void SimpleLabelNode::addString(SimpleLabelAnchor anchor, std::string str)
{
    labelStrs[anchor] += str.append("\n");
}

CCPoint SimpleLabelNode::getPointForAnchor(SimpleLabelAnchor anchor)
{
    CCPoint point = CCPointZero;

    if (anchor == SimpleLabelAnchor::BottomMiddle || anchor == SimpleLabelAnchor::Middle || anchor == SimpleLabelAnchor::TopMiddle)
        point.x = 0.5f;

    if (anchor == SimpleLabelAnchor::BottomRight || anchor == SimpleLabelAnchor::MiddleRight || anchor == SimpleLabelAnchor::TopRight)
        point.x = 1;
        
    
    if (anchor == SimpleLabelAnchor::Middle || anchor == SimpleLabelAnchor::MiddleLeft || anchor == SimpleLabelAnchor::MiddleRight)
        point.y = 0.5f;

    if (anchor == SimpleLabelAnchor::TopLeft || anchor == SimpleLabelAnchor::TopMiddle || anchor == SimpleLabelAnchor::TopRight)
        point.y = 1;

    return point;
}

void SimpleLabelNode::setContentSize(const CCSize& var)
{
    CCLayer::setContentSize(var);

    for (size_t i = 0; i < labels.size(); i++)
    {
        auto ac = as<SimpleLabelAnchor>(i);
        auto point = getPointForAnchor(ac);

        labels[ac]->setPosition(var * point);
    }
}

void SimpleLabelNode::update(float dt)
{
    if (!isVisible())
        return;

    for (size_t i = 0; i < 9; i++)
    {
        auto ac = as<SimpleLabelAnchor>(i);
        
        labelStrs[ac] = "";
    }
    

    addString(SimpleLabelAnchor::TopLeft, "");


    for (size_t i = 0; i < 9; i++)
    {
        auto ac = as<SimpleLabelAnchor>(i);

        labels[ac]->setString(labelStrs[ac].c_str());
        labels[ac]->setScale(0.5f);
    }
}