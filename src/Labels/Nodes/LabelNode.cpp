#include "LabelNode.hpp"

bool LabelNode::init()
{
    setup();

    return true;
}

void LabelNode::updateGeneral(float dt)
{
    this->setScale(config.scale);

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
    return getActionByTag(80085);
}