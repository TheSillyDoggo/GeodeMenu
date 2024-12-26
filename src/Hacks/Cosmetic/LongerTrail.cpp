#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCMotionStreak.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    virtual void update(float delta)
    {
        if (getUserObject("ignore-longer-trail"_spr))
            delta *= 3;

        CCMotionStreak::update(delta / 3);
    }

    QOLMOD_MOD_HOOK("longer-trail", "cocos2d::CCMotionStreak::update")
};