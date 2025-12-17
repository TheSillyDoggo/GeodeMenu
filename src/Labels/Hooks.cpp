#include "Hooks.hpp"
#include "Nodes/LabelContainerLayer.hpp"

bool LabelPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    m_uiLayer->addChild(LabelContainerLayer::create());

    return true;
}