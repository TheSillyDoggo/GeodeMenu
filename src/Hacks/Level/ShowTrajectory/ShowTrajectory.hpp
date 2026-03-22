#pragma once

#include "../../../Client/Module.hpp"
#include "../../../Client/FloatSliderModule.hpp"
#include "../../../Client/ColourModule.hpp"
#include "../../../Client/InputModule.hpp"
#include "Hooks.hpp"

using namespace geode::prelude;

class ShowTrajectory : public Module
{
    public:
        MODULE_SETUP(ShowTrajectory)
        {
            setID("show-trajectory");
            setCategory("Level");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

class ShowTrajectoryHold : public ColourModule
{
    public:
        MODULE_SETUP(ShowTrajectoryHold)
        {
            setName("Hold Colour:");
            setID("show-trajectory/hold-colour");
            setDefaultConfig({ ccc3(0, 255, 0)} );
            setPriority(1);
        }
};

class ShowTrajectoryRelease : public ColourModule
{
    public:
        MODULE_SETUP(ShowTrajectoryRelease)
        {
            setName("Release Colour:");
            setID("show-trajectory/release-colour");
            setDefaultConfig({ ccc3(0, 115, 0)} );
            setPriority(2);
        }
};

SUBMIT_HACK(ShowTrajectory)
SUBMIT_OPTION(ShowTrajectory, ShowTrajectoryHold)
SUBMIT_OPTION(ShowTrajectory, ShowTrajectoryRelease)