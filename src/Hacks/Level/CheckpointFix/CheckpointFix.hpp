#pragma once

#include <Module.hpp>

using namespace geode::prelude;

class CheckpointFix : public Module
{
    public:
        MODULE_SETUP(CheckpointFix)
        {
            setID("checkpoint-fix");
            setCategory("Level");
        }
};