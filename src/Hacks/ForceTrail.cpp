#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCMotionStreak)
{
    virtual void update(float delta)
    {
        if (Client::GetModuleEnabled("trail-off"))
            m_bStroke = false;

        if (Client::GetModuleEnabled("trail-on"))
            m_bStroke = true;

        CCMotionStreak::update(delta);
    }
};