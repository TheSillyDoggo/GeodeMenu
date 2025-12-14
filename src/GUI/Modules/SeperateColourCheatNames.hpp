#pragma once

#include "../../Client/Module.hpp"
#include "../../Client/ColourModule.hpp"

using namespace geode::prelude;

class SeperateColourCheatNames : public Module
{
    public:
        MODULE_SETUP(SeperateColourCheatNames)
        {
            setName("Seperate colour cheat names");
            setID("seperate-colour-cheat-name");
            setCategory("Config/Menu");
            setDescription("Changes the colour of mod names if it is a cheat");
            setDefaultEnabled(true);
        }
};

class CheatNameColour : public ColourModule
{
    public:
        MODULE_SETUP(CheatNameColour)
        {
            setName("Colour:");
            setID("seperate-colour-cheat-name/colour");
            setDefaultConfig({ ccc3(255, 100, 100) });
        }
};

SUBMIT_HACK(SeperateColourCheatNames);
SUBMIT_OPTION(SeperateColourCheatNames, CheatNameColour);