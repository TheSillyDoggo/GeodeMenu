#pragma once

#include "../../Client/Module.hpp"
#include <PopupBase.hpp>

using namespace geode::prelude;

class BlurMenuBG : public Module
{
    public:
        MODULE_SETUP(BlurMenuBG)
        {
            setName("Blur Background");
            setID("blur-background");
            setDescription("Blurs the background behind the mod menu");
        }

        virtual void onToggle()
        {
            PopupBase::updateAllBlur();
        }
};

SUBMIT_HACK(BlurMenuBG);