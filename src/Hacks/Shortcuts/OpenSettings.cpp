#pragma once

#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

class OpenSettings : public ButtonModule
{
    public:
        MODULE_SETUP(OpenSettings)
        {
            setID("shortcut/open-settings");
            setCategory("Universal");

            setPriority(sortPriority + 2);
        }

        virtual void onClick()
        {
            OptionsLayer::create()->showLayer(false);
        }
};

SUBMIT_HACK(OpenSettings);