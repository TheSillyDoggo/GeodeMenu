#include <Geode/Geode.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* noWaveMod = nullptr;
SliderModule* noWaveEffect = nullptr;

class $modify (HardStreak)
{
    void updateStroke(float p0)
    {
        if (!noWaveMod)
        {
            noWaveMod = Module::get("no-wave-pulse");
            noWaveEffect = as<SliderModule*>(noWaveMod->options[0]);
        }

        if (noWaveMod->enabled)
            m_pulseSize = noWaveEffect->value + 0.5f;

        HardStreak::updateStroke(p0);
    }
};