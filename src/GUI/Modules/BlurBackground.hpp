#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class BlurMenuBG : public Module
{
    public:
        MODULE_SETUP(BlurMenuBG)
        {
            setName("Blur Background");
            setID("blur-background");
            setDescription("Blurs the background behind the mod menu");

            // todo
            setDisabled(true);
            setDisabledMessage("Coming soon!");
        }
};

SUBMIT_HACK(BlurMenuBG);