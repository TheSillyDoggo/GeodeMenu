#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#ifndef GEODE_IS_ANDROID
#define APPLY_SHAKE_INLINE
#endif

class $modify (GJBaseGameLayer)
{
    #ifdef APPLY_SHAKE_INLINE

    void updateCamera(float physDeltaX60)
    {
        m_gameState.m_cameraShakeFactor = 0;
        m_gameState.m_cameraShakeEnabled = false;

        GJBaseGameLayer::updateCamera(physDeltaX60);
    }

    #else

    void applyShake(cocos2d::CCPoint& p0)
    {
        
    }

    #endif

    QOLMOD_MOD_ALL_HOOKS("no-shake")
};

class $modify (GJBaseGameLayer)
{
    void updateStaticCameraPos(cocos2d::CCPoint p0, bool p1, bool p2, bool p3, float p4, int p5, float p6)
    {
        
    }

    QOLMOD_MOD_ALL_HOOKS("no-static")
};