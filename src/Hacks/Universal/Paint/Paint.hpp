#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class PaintEnabled : public Module
{
    public:
        MODULE_SETUP(PaintEnabled)
        {
            setID("paint");
            setCategory("Universal");
            setPriority(5);
        }
};

/*class PaintVerticalControl : public Module
{
    public:
        MODULE_SETUP(PaintVerticalControl)
        {
            setID("paint/vertical-control");
            setDefaultEnabled(true);
        }

        virtual void onToggle();
};*/

SUBMIT_HACK(PaintEnabled)
// SUBMIT_OPTION(PaintEnabled, PaintVerticalControl)