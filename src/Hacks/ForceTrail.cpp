#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* on = nullptr;
Module* off = nullptr;

class $modify (CCMotionStreak)
{
    virtual void update(float delta)
    {
        if (Client::GetModuleEnabled("trail-on"))
            m_bStroke = true;

        if (Client::GetModuleEnabled("trail-off"))
            m_bStroke = false;

        CCMotionStreak::update(delta);
    }
};