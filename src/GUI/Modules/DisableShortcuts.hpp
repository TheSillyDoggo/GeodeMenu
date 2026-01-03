#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class DisableShortcuts : public Module
{
    public:
        MODULE_SETUP(DisableShortcuts)
        {
            setID("disable-shortcuts");
            setCategory("Config/Menu");
            setPriority(sortPriority + 2);
        }
};


SUBMIT_HACK(DisableShortcuts);