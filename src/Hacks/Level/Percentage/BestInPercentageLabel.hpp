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

class BestInPercentageHideStartpos : public Module
{
    public:
        MODULE_SETUP(BestInPercentageHideStartpos)
        {
            setID("best-in-percentage/hide-from-startpos");
        }
};

SUBMIT_HACK(BestInPercentage);
SUBMIT_OPTION(BestInPercentage, BestInPercentageHideStartpos);

