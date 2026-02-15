#include "BetterGradientNode.hpp"

using namespace geode::prelude;

GradientPoint::GradientPoint(float position, cocos2d::ccColor4B colour)
{
    this->position = position;
    this->colour = colour;
}

bool BetterGradientNode::init()
{
    if (!CCNode::init())
        return false;

    return true;
}

void BetterGradientNode::draw()
{
    
}
/*
#include <Geode/modify/MenuLayer.hpp>

class $modify (MenuLayer)
{
    virtual bool init()
    {
        MenuLayer::init();

        this->addChild(BetterGradientNode::create());

        return true;
    }
};*/