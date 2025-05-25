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
        if (Module::get("trail-on")->enabled)
            m_bStroke = true;

        if (Module::get("trail-off")->enabled)
            m_bStroke = false;

        CCMotionStreak::update(delta);
    }
};