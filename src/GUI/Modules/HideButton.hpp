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
            setCategory("Config");
            setDescription("Hides the button to open the mod");

            #ifdef GEODE_IS_MOBILE
            setDefaultEnabled(true);
            setDisabled(true);
            setDisabledMessage("You can't hide the button on mobile, silly :P");
            #endif
        }
};

SUBMIT_HACK(HideButton);