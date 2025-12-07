#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class SmoothMoveButton : public Module
{
    public:
        MODULE_SETUP(SmoothMoveButton)
        {
            setName("Smooth Move");
            setID("smooth-move");
            setCategory("Config/Button");
            setDescription("Moves the button smoothly to where you put it");
            setDefaultEnabled(true);
        }
};



SUBMIT_HACK(SmoothMoveButton);