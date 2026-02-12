#include "LabelNode.hpp"
#include "../../Hacks/Level/Noclip/Noclip.hpp"
#include "TextLabelNode.hpp"
#include "KeyCheckerNode.hpp"
#include "ImageNode.hpp"

LabelNode* LabelNode::createForType(LabelType type)
{
    switch (type)
    {
        case LabelType::KeyChecker:
            return KeyCheckerNode::create();

        case LabelType::Image:
            return ImageNode::create();

        default:
            return TextLabelNode::create();
    }
}

bool LabelNode::init()
{
    setup();

    return true;
}

void LabelNode::updateGeneral(float dt)
{
    this->setVisible(isVisible());
    this->setScale(config.scale);

    if (isVisible())
        update(dt);
}

void LabelNode::setup()
{

}

void LabelNode::update(float dt)
{

}

void LabelNode::labelConfigUpdated()
{

}

void LabelNode::setLabelConfig(LabelConfig config)
{
    this->config = config;
    labelConfigUpdated();
}

const LabelConfig& LabelNode::getLabelConfig()
{
    return config;
}

bool LabelNode::isActionActive()
{
    return getActionByTag(80085) || getActionByTag(800851);
}

bool LabelNode::isVisible()
{
    if (!config.visible)
        return false;

    if (config.noclipOnly)
        return Noclip::get()->getRealEnabled();

    return true;
}

void LabelNode::visit(void)
{
    if (!isVisible())
        return;

    CCNode::visit();
}

void LabelNode::onEventTriggered(LabelEventType type)
{

}