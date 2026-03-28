#include "ShowTouchLayer.hpp"
#include <Geode/Geode.hpp>
#include "TouchTrail.hpp"

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
    trails[id] = TouchTrail::create();
    this->addChild(trails[id]);

    trails[id]->setPosition(point);
}

void ShowTouchLayer::touchMoved(int id, CCPoint point)
{
    trails[id]->setPosition(point);
}

void ShowTouchLayer::touchEnded(int id, CCPoint point)
{
    this->removeChild(trails[id]);
    trails.erase(id);
}

void ShowTouchLayer::touchCancelled(int id, CCPoint point)
{
    touchEnded(id, point);
}