#include "Hooks.hpp"
#include "SimpleLabelNode.hpp"

bool LabelPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    this->addChild(SimpleLabelNode::create());

    return true;
}