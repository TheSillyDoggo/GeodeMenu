#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    virtual void update(float delta)
    {
        CCMotionStreak::update(delta / 3);
    }

    QOLMOD_MOD_HOOK("longer-trail", "cocos2d::CCMotionStreak::update")
};