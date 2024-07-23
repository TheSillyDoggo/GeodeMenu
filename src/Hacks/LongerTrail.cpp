#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    struct Fields
    {
        int update = 3;
    };

    virtual void update(float delta)
    {
        if (m_fields->update != 3)
        {
            m_fields->update++;
            return;
        }
        else
        {
            m_fields->update = 0;
            CCMotionStreak::update(delta);
        }
    }

    QOLMOD_MOD_HOOK("longer-trail", "cocos2d::CCMotionStreak::update")
};