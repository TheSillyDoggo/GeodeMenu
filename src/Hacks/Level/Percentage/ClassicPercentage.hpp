#pragma once

#include "../../../Client/InputModule.hpp"
#include "../../Utils/PlayLayer.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class ClassicPercentage : public Module
{
    public:
        MODULE_SETUP(ClassicPercentage)
        {
            setName("Classic Percentage");
            setID("classic-percentage");
            setCategory("Level");
            setDescription("Changes the progress bar percentage to be calculated based off position, rather than time, similar to pre 2.2");
        }
};

SUBMIT_HACK(ClassicPercentage);