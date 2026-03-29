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

void ShowTouchLayer::touchBegan(int id, CCPoint point)
{
    if (trails.contains(id))
        touchEnded(id, point);
    
    trails[id] = TouchTrail::create();
    this->addChild(trails[id]);

    trails[id]->setPosition(point);
}

void ShowTouchLayer::touchMoved(int id, CCPoint point)
{
    if (!trails.contains(id))
        return;

    trails[id]->setPosition(point);
}

void ShowTouchLayer::touchEnded(int id, CCPoint point)
{
    if (!trails.contains(id))
        return;

    this->removeChild(trails[id]);
    trails.erase(id);
}

void ShowTouchLayer::touchCancelled(int id, CCPoint point)
{
    touchEnded(id, point);
}

void ShowTouchLayer::visit()
{
    if (ShowTouches::get()->getRealEnabled())
        CCNode::visit();
}