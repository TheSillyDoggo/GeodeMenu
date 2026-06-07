#include "GestureManager.hpp"

using namespace geode::prelude;
using namespace qolmod;

GestureManager* GestureManager::get()
{
    static GestureManager* instance = []{
        auto pRet = new GestureManager();
        pRet->onEnter();
        return pRet;
    }();

    return instance;
}

void GestureManager::draw()
{
    return;
    auto rect = geode::utils::getSafeAreaRect();

    glLineWidth(3);
    ccDrawColor4B(0, 0, 255, 255);
    ccDrawRect(rect.origin, rect.origin + rect.size);
}

bool GestureManager::touchBegan(qolmod::Touch* touch)
{
    return false;
    if (trackedTouches.contains(touch->id))
        trackedTouches.erase(touch->id);

    for (auto node : getNodes())
    {
        if (node->touchBegan(touch))
        {
            trackedTouches[touch->id] = node;
            return true;
        }
    }

    return false;
}

bool GestureManager::touchMoved(qolmod::Touch* touch)
{
    if (trackedTouches.contains(touch->id))
    {
        trackedTouches[touch->id]->touchMoved(touch);
        return true;
    }

    return false;
}

bool GestureManager::touchEnded(qolmod::Touch* touch)
{
    if (trackedTouches.contains(touch->id))
    {
        trackedTouches[touch->id]->touchEnded(touch);
        trackedTouches.erase(touch->id);
        return true;
    }

    return false;
}

bool GestureManager::touchCancelled(qolmod::Touch* touch)
{
    if (trackedTouches.contains(touch->id))
    {
        trackedTouches[touch->id]->touchCancelled(touch);
        trackedTouches.erase(touch->id);
        return true;
    }

    return false;
}

std::vector<GestureNode*> GestureManager::getNodes()
{
    auto vec = CCArrayExt<GestureNode*>(getChildren());

    return std::vector<GestureNode*>(vec.begin(), vec.end());
}


/*$on_game(Loaded)
{
    GestureManager::get()->addChild([]{
        auto test = new GestureNode();
        test->init();
        test->autorelease();
        return test;
    }());
}*/