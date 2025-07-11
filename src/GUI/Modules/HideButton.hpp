#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class HideButton : public Module
{
    public:
        MODULE_SETUP(HideButton)
        {
            setName("Hide Button");
            setID("hide-btn");
            setCategory("Universal");
            setDescription("Hide button help");

            #ifdef GEODE_IS_DESKTOP
            setDefaultEnabled(true);
            #endif
        }
};

SUBMIT_HACK(HideButton);