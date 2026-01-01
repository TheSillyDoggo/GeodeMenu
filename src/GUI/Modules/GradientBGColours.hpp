#pragma once

#include "../../Client/ColourModule.hpp"
#include "../../Client/RotationModule.hpp"

using namespace geode::prelude;

class GradientBGStart : public ColourModule
{
    public:
        MODULE_SETUP(GradientBGStart)
        {
            setName("Top Colour:");
            setID("gradient-bg/start-colour");

            setDefaultConfig({ ccc3(255, 0, 0), 1.0f, 1.0f, Player1 });
        }
};

class GradientBGEnd : public ColourModule
{
    public:
        MODULE_SETUP(GradientBGEnd)
        {
            setName("Bottom Colour:");
            setID("gradient-bg/end-colour");

            setDefaultConfig({ ccc3(0, 255, 0), 1.0f, 1.0f, Player2 });
        }
};

class GradientBGDirection : public RotationModule
{
    public:
        MODULE_SETUP(GradientBGDirection)
        {
            setID("gradient-bg/rotation");

            setDefaultValue(90);
        }
};

SUBMIT_HACK(GradientBGStart);
SUBMIT_HACK(GradientBGEnd);
SUBMIT_HACK(GradientBGDirection);