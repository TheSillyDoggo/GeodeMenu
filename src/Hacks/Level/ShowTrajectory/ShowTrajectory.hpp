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
            setPriority(13);
        }
};

class ShowTrajectoryP1 : public Module
{
    public:
        MODULE_SETUP(ShowTrajectoryP1)
        {
            setID("show-trajectory/player1");
            setPriority(1);
            setDefaultEnabled(true);
        }
};

class ShowTrajectoryP2 : public Module
{
    public:
        MODULE_SETUP(ShowTrajectoryP2)
        {
            setID("show-trajectory/player2");
            setPriority(2);
            setDefaultEnabled(true);
        }
};

class ShowTrajectoryP1Hold : public ColourModule
{
    public:
        MODULE_SETUP(ShowTrajectoryP1Hold)
        {
            setID("show-trajectory/player1/hold-colour");
            setDefaultConfig({ ccc3(0, 255, 0)} );
            setPriority(1);
        }
};

class ShowTrajectoryP1Release : public ColourModule
{
    public:
        MODULE_SETUP(ShowTrajectoryP1Release)
        {
            setID("show-trajectory/player1/release-colour");
            setDefaultConfig({ ccc3(0, 115, 0)} );
            setPriority(2);
        }
};

class ShowTrajectoryP2Hold : public ColourModule
{
    public:
        MODULE_SETUP(ShowTrajectoryP2Hold)
        {
            setID("show-trajectory/player2/hold-colour");
            setDefaultConfig({ ccc3(0, 255, 0)} );
            setPriority(1);
        }
};

class ShowTrajectoryP2Release : public ColourModule
{
    public:
        MODULE_SETUP(ShowTrajectoryP2Release)
        {
            setID("show-trajectory/player2/release-colour");
            setDefaultConfig({ ccc3(0, 115, 0)} );
            setPriority(2);
        }
};

class ShowTrajectoryP1Trail : public Module
{
    public:
        MODULE_SETUP(ShowTrajectoryP1Trail)
        {
            setID("show-trajectory/player1/hitbox-trail");
            setPriority(3);
        }
};

class ShowTrajectoryP2Trail : public Module
{
    public:
        MODULE_SETUP(ShowTrajectoryP2Trail)
        {
            setID("show-trajectory/player2/hitbox-trail");
            setPriority(3);
        }
};

SUBMIT_HACK(ShowTrajectory)
SUBMIT_OPTION(ShowTrajectory, ShowTrajectoryP1)
SUBMIT_OPTION(ShowTrajectory, ShowTrajectoryP2)
SUBMIT_OPTION(ShowTrajectoryP1, ShowTrajectoryP1Hold)
SUBMIT_OPTION(ShowTrajectoryP2, ShowTrajectoryP2Hold)
SUBMIT_OPTION(ShowTrajectoryP1, ShowTrajectoryP1Release)
SUBMIT_OPTION(ShowTrajectoryP2, ShowTrajectoryP2Release)
SUBMIT_OPTION(ShowTrajectoryP1, ShowTrajectoryP1Trail)
SUBMIT_OPTION(ShowTrajectoryP2, ShowTrajectoryP2Trail)