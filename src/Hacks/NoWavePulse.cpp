#include <Geode/Geode.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (HardStreak)
{
    void updateStroke(float p0)
    {
        m_pulseSize = 1;

        HardStreak::updateStroke(p0);
    }
};