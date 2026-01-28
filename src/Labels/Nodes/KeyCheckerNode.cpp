#include "KeyCheckerNode.hpp"
#include "../../Utils/ColourUtils.hpp"
#include <BlurAPI.hpp>

void KeyCheckerNode::setup()
{
    layer = CCLayerColor::create(ccc4(0, 0, 0, 100));
    layer->setAnchorPoint(ccp(0, 0));
    layer->setPosition(ccp(0, 0));
    layer->setContentSize(ccp(35, 35));

    // BlurAPI::addBlur(layer);

    labelUpArrow = AdvLabelBMFont::createWithString(">", "chatFont.fnt");
    labelUpArrow->setRotation(-90);

    this->addChild(layer);
    this->addChild(labelUpArrow);
}

void KeyCheckerNode::labelConfigUpdated()
{

}

void KeyCheckerNode::update(float dt)
{
    setContentSize(layer->getContentSize());
    labelUpArrow->setPosition(getContentSize() / 2);

    labelUpArrow->setColor((UILayer::get()->m_p1Jumping || UILayer::get()->m_p2Jumping) ? ColourUtils::get()->getPastel("keychecker") : ccWHITE);
}