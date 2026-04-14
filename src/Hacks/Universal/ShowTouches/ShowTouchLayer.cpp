#include "ShowTouchLayer.hpp"
#include <Geode/Geode.hpp>
#include "TouchTrail.hpp"
#include "Colours.hpp"

using namespace geode::prelude;
using namespace qolmod;

ShowTouchLayer* ShowTouchLayer::get()
{
    static ShowTouchLayer* instance = []
    {
        auto pRet = new ShowTouchLayer();
        pRet->onEnter();
        return pRet;
    }();

    return instance;
}

void ShowTouchLayer::touchBegan(qolmod::Touch* touch)
{
    if (trails.contains(touch->id))
        touchEnded(touch);
    
    trails[touch->id] = TouchTrail::create();
    this->addChild(trails[touch->id]);

    trails[touch->id]->setPosition(touch->location);
}

void ShowTouchLayer::touchMoved(qolmod::Touch* touch)
{
    if (!trails.contains(touch->id))
        return;

    trails[touch->id]->setPosition(touch->location);
}

void ShowTouchLayer::touchEnded(qolmod::Touch* touch)
{
    if (!trails.contains(touch->id))
        return;

    this->removeChild(trails[touch->id]);
    trails.erase(touch->id);
}

void ShowTouchLayer::touchCancelled(qolmod::Touch* touch)
{
    touchEnded(touch);
}

void ShowTouchLayer::visit()
{
    if (ShowTouches::get()->getRealEnabled())
        CCNode::visit();
}