#pragma once

#include "../../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class BestInPercentage : public Module
{
    public:
        MODULE_SETUP(BestInPercentage)
        {
            setName("Best In Percentage");
            setID("best-in-percentage");
            setCategory("Level");
            setDescription("Shows the best percentage in the progress bar");
        }
};

SUBMIT_HACK(BestInPercentage);

