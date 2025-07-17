#pragma once

#include "../../Client/Module.hpp"

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
            setDefaultEnabled(true);
        }
};


SUBMIT_HACK(ForceSafeMode);