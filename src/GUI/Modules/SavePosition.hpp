#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class SavePosition : public Module
{
    public:
        MODULE_SETUP(SavePosition)
        {
            setName("Save Position");
            setID("save-pos");
            setCategory("Config");
            setDescription("Save position help");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(SavePosition);