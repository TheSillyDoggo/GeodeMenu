#pragma once

#include "../../Client/InputModule.hpp"

using namespace geode::prelude;

class ForceSafeMode : public Module
{
    public:
        MODULE_SETUP(ForceSafeMode)
        {
            setName("Safe Mode");
            setID("safe-mode");
            setCategory("Universal");
            setDescription("Disables all level progress and stats");
            setPriority(2);
        }
};

class SafeModeChangeText : public Module
{
    public:
        MODULE_SETUP(SafeModeChangeText)
        {
            setName("Change Text");
            setID("safe-mode/change-text");
            setDescription("Changes the end screen text to make it obvious if the run is cheated");
            setDefaultEnabled(true);
            setPriority(1);
        }
};

class SafeModeText : public InputModule
{
    public:
        MODULE_SETUP(SafeModeText)
        {
            setName("Text");
            setID("safe-mode/text");
            
            setDefaultString("Safe Mode :3");
            setPlaceholderString("Text");

            setStringFilter("");
            setMaxCharCount(50);
            setPriority(2);
        }
};

class SafeModeEndScreenCheatIndicator : public Module
{
    public:
        MODULE_SETUP(SafeModeEndScreenCheatIndicator)
        {
            setID("safe-mode/end-screen-cheat-indicator");
            setDefaultEnabled(true);
            setPriority(3);
        }
};

SUBMIT_HACK(ForceSafeMode);
SUBMIT_OPTION(ForceSafeMode, SafeModeChangeText);
SUBMIT_OPTION(ForceSafeMode, SafeModeText);
SUBMIT_OPTION(ForceSafeMode, SafeModeEndScreenCheatIndicator);