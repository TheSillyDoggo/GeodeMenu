#pragma once

#include "../../../Client/Module.hpp"
#include "../../../Client/FloatSliderModule.hpp"
#include "../../../Client/ColourModule.hpp"
#include "../../../Client/InputModule.hpp"
#include "Hooks.hpp"

using namespace geode::prelude;

class Noclip : public Module
{
    public:
        MODULE_SETUP(Noclip)
        {
            setID("noclip");
            setCategory("Level");
            setSafeModeTrigger(SafeModeTrigger::Custom);
            setSafeModeCustom([this]
            {
                if (auto pl = PlayLayer::get())
                {
                    if (reinterpret_cast<NoclipBaseGameLayer*>(pl)->m_fields->hasDiedThisAttempt)
                        return true;
                }

                return false;
            });
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



class NoclipPlayer1 : public Module
{
    public:
        MODULE_SETUP(NoclipPlayer1)
        {
            setID("noclip/player1");
            setPriority(4);
            setDefaultEnabled(true);
        }
};

class NoclipPlayer2 : public Module
{
    public:
        MODULE_SETUP(NoclipPlayer2)
        {
            setID("noclip/player2");
            setPriority(5);
            setDefaultEnabled(true);
        }
};

class NoclipPlayer1UseMinAcc : public Module
{
    public:
        MODULE_SETUP(NoclipPlayer1UseMinAcc)
        {
            setID("noclip/player1/use-min-accuracy");
            setPriority(1);
        }
};

class NoclipPlayer2UseMinAcc : public Module
{
    public:
        MODULE_SETUP(NoclipPlayer2UseMinAcc)
        {
            setID("noclip/player2/use-min-accuracy");
            setPriority(1);
        }
};

class NoclipPlayer1MinAcc : public InputModule
{
    public:
        MODULE_SETUP(NoclipPlayer1MinAcc)
        {
            setID("noclip/player1/min-accuracy");
            setPriority(2);

            setDefaultString("100");
            setPlaceholderString("Percent");
            setHint("percentage");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

class NoclipPlayer2MinAcc : public InputModule
{
    public:
        MODULE_SETUP(NoclipPlayer2MinAcc)
        {
            setID("noclip/player2/min-accuracy");
            setPriority(2);

            setDefaultString("100");
            setPlaceholderString("Percent");
            setHint("percentage");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

class NoclipPlayer1UseMaxDeaths : public Module
{
    public:
        MODULE_SETUP(NoclipPlayer1UseMaxDeaths)
        {
            setID("noclip/player1/use-max-deaths");
            setPriority(3);
        }
};

class NoclipPlayer2UseMaxDeaths : public Module
{
    public:
        MODULE_SETUP(NoclipPlayer2UseMaxDeaths)
        {
            setID("noclip/player2/use-max-deaths");
            setPriority(3);
        }
};

class NoclipPlayer1MaxDeaths : public InputModule
{
    public:
        MODULE_SETUP(NoclipPlayer1MaxDeaths)
        {
            setID("noclip/player1/max-deaths");
            setPriority(4);

            setDefaultString("0");
            setPlaceholderString("Deaths");

            setStringFilter("1234567890");
            setMaxCharCount(5);
        }
};

class NoclipPlayer2MaxDeaths : public InputModule
{
    public:
        MODULE_SETUP(NoclipPlayer2MaxDeaths)
        {
            setID("noclip/player2/max-deaths");
            setPriority(4);

            setDefaultString("0");
            setPlaceholderString("Deaths");

            setStringFilter("1234567890");
            setMaxCharCount(5);
        }
};

SUBMIT_HACK(Noclip);
SUBMIT_OPTION(Noclip, NoclipTintOnDeath);
SUBMIT_OPTION(Noclip, NoclipTintOpacity);
SUBMIT_OPTION(Noclip, NoclipTintColour);
SUBMIT_OPTION(Noclip, NoclipPlayer1);
SUBMIT_OPTION(Noclip, NoclipPlayer2);
SUBMIT_OPTION(NoclipPlayer1, NoclipPlayer1UseMinAcc);
SUBMIT_OPTION(NoclipPlayer2, NoclipPlayer2UseMinAcc);
SUBMIT_OPTION(NoclipPlayer1, NoclipPlayer1MinAcc);
SUBMIT_OPTION(NoclipPlayer2, NoclipPlayer2MinAcc);
SUBMIT_OPTION(NoclipPlayer1, NoclipPlayer1UseMaxDeaths);
SUBMIT_OPTION(NoclipPlayer2, NoclipPlayer2UseMaxDeaths);
SUBMIT_OPTION(NoclipPlayer1, NoclipPlayer1MaxDeaths);
SUBMIT_OPTION(NoclipPlayer2, NoclipPlayer2MaxDeaths);