#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class AutoSafeMode : public Module
{
    public:
        MODULE_SETUP(AutoSafeMode)
        {
            setName("Auto Safe Mode");
            setID("auto-safe-mode");
            setCategory("Universal");
            setDescription("Automatically enables safe mode when a cheat is enabled");
            setDefaultEnabled(true);
            setDisableWarning("Disabling this mod will allow you to complete levels with <cr>cheats enabled</c>.\nThis may get you banned from the <cg>in-game</c> leaderboards.\nIf you disable <cc>Auto Safe Mode</c>, it is <cj>your</c> fault if you get <cr>banned</c>.");
            setPriority(1);
        }
};

SUBMIT_HACK(AutoSafeMode);