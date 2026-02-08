#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class HidePauseMenu : public Module
{
    public:
        MODULE_SETUP(HidePauseMenu)
        {
            setID("hide-pause-menu");
            setCategory("Universal");
        }
};

SUBMIT_HACK(HidePauseMenu)