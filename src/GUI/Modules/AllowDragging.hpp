#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class AllowDragging : public Module
{
    public:
        MODULE_SETUP(AllowDragging)
        {
            setName("Allow Dragging");
            setID("allow-dragging");
            setCategory("Universal");
            setDescription("Allow dragging help");
            setDefaultEnabled(true);
        }
};


SUBMIT_HACK(AllowDragging);