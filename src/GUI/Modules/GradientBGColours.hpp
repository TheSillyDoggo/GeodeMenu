#pragma once

#include "../../Client/ColourModule.hpp"

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

SUBMIT_HACK(GradientBGStart);
SUBMIT_HACK(GradientBGEnd);