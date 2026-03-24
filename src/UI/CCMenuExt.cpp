#include "CCMenuExt.hpp"
#include <Num.hpp>

using namespace geode::prelude;
using namespace qolmod;

void CCMenuExt::setUseRect(bool use)
{
    this->useArea = use;
}

void CCMenuExt::setRect(CCRect rect)
{
    setUseRect(true);
    this->area = rect;
}

void CCMenuExt::setLinkedRect(cocos2d::CCNode* node)
{
    setUseRect(node != nullptr);
    this->linkedRect = node;
}

bool CCMenuExt::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (useArea)
    {
        if (linkedRect)
        {
            if (!getWorldSpaceBoundingBox(linkedRect).containsPoint(touch->getLocation()))
                return false;
        }
        else
        {
            if (!area.containsPoint(touch->getLocation()))
                return false;
        }
    }

    return CCMenu::ccTouchBegan(touch, event);
}

void CCMenuExt::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCMenu::ccTouchEnded(touch, event);
}

void CCMenuExt::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    CCMenu::ccTouchCancelled(touch, event);
}

void CCMenuExt::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCMenu::ccTouchMoved(touch, event);
}