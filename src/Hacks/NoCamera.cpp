#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (EffectGameObject)
{
    //edit_eStaticBtn_001.png
};

Module* shake = nullptr;
Module* stati = nullptr;

class $modify (GJBaseGameLayer)
{
    #ifdef GEODE_IS_ANDROID
    void applyShake(cocos2d::CCPoint& p0)
    {
        if (!shake)
            shake = Client::GetModule("no-shake");

        if (!shake->enabled)
            GJBaseGameLayer::applyShake(p0);
    }
    #endif

    void updateStaticCameraPos(cocos2d::CCPoint p0, bool p1, bool p2, bool p3, float p4, int p5, float p6)
    {
        if (!stati)
            stati = Client::GetModule("no-static");

        if (!stati->enabled)
            GJBaseGameLayer::updateStaticCameraPos(p0, p1, p2, p3, p4, p5, p6);
    }
};