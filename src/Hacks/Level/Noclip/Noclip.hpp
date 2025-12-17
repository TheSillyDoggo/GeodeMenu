#pragma once

#include "../../../Client/Module.hpp"
#include "../../../Client/FloatSliderModule.hpp"
#include "../../../Client/ColourModule.hpp"

using namespace geode::prelude;

class Noclip : public Module
{
    public:
        MODULE_SETUP(Noclip)
        {
            setName("Noclip");
            setID("noclip");
            setCategory("Level");
            setDescription("Prevents the player from dying");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
            setPriority(1);
        }
};

class NoclipTintOnDeath : public Module
{
    public:
        MODULE_SETUP(NoclipTintOnDeath)
        {
            setName("Tint on death");
            setID("noclip/tint-on-death");
            setDescription("Tints the screen when you die");
            setPriority(1);
        }
};

class NoclipTintOpacity : public FloatSliderModule
{
    public:
        MODULE_SETUP(NoclipTintOpacity)
        {
            setName("Tint Opacity:");
            setID("noclip/tint-opacity");
            setDefaultValue(0.25f);
            setSnapValues({ 0.25f });
            setRange(0.05f, 1.0f);
            setPriority(2);
        }
};

class NoclipTintColour : public ColourModule
{
    public:
        MODULE_SETUP(NoclipTintColour)
        {
            setName("Tint Colour:");
            setID("noclip/tint-colour");
            setDefaultConfig({ ccc3(255, 0, 0)} );
            setPriority(3);
        }
};

SUBMIT_HACK(Noclip);
SUBMIT_OPTION(Noclip, NoclipTintOnDeath);
SUBMIT_OPTION(Noclip, NoclipTintOpacity);
SUBMIT_OPTION(Noclip, NoclipTintColour);