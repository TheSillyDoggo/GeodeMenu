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
            setCategory("Config/Button");
            setDescription("Hides the button to open the mod");
            setDefaultEnabled(true);

            #ifdef GEODE_IS_MOBILE
            setDefaultEnabled(false);
            setDisabled(true);
            setDisabledMessage("You can't hide the button on mobile, silly :P");
            #endif
        }
};

SUBMIT_HACK(HideButton);