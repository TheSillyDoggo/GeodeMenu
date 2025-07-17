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
        }
};

SUBMIT_HACK(AutoSafeMode);