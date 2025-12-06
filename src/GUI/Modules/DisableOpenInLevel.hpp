#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class DisableOpenInLevel : public Module
{
    public:
        MODULE_SETUP(DisableOpenInLevel)
        {
            setName("Disable Opening In Level");
            setID("disable-open-in-level");
            setCategory("Config/Menu");
            setDescription("Makes it so you can't open the mod menu while in a level.\nJust incase you accidentally bump the tab button");
            setDefaultEnabled(true);
        }
};


SUBMIT_HACK(DisableOpenInLevel);