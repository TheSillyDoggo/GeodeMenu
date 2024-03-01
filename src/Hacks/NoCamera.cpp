#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    TodoReturn applyShake(cocos2d::CCPoint& p0)
    {
        if (!Client::GetModuleEnabled("no-shake"))
            GJBaseGameLayer::applyShake(p0);
    }

    TodoReturn updateStaticCameraPos(cocos2d::CCPoint p0, bool p1, bool p2, bool p3, float p4, int p5, float p6)
    {
        if (!Client::GetModuleEnabled("no-static"))
            GJBaseGameLayer::updateStaticCameraPos(p0, p1, p2, p3, p4, p5, p6);
    }
    /*TodoReturn updateCamera(float p0) // delta probably ?
    {
        if (!Client::GetModuleEnabled("no-camera"))
            GJBaseGameLayer::updateCamera(p0);
    }*/
};