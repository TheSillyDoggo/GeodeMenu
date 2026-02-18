#pragma once

#include "../../Client/InputModule.hpp"

using namespace geode::prelude;

class SafeModeDisableCheats : public Module
{
    public:
        MODULE_SETUP(SafeModeDisableCheats)
        {
            setID("safe-mode/disable-cheats-ui");
            setPriority(4);
        }
};