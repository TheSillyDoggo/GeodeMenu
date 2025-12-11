#pragma once

#include "../../../Client/Module.hpp"

using namespace geode::prelude;

class ShowHitboxes : public Module
{
    public:
        MODULE_SETUP(ShowHitboxes)
        {
            setName("Show Hitboxes");
            setID("show-hitboxes");
            setCategory("Level");
            setDescription("Shows object hitboxes");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
            setPriority(3);
        }
};

class ShowHitboxesOnDeath : public Module
{
    public:
        MODULE_SETUP(ShowHitboxesOnDeath)
        {
            setName("Show Hitboxes On Death");
            setID("show-hitboxes-on-death");
            setCategory("Level");
            setDescription("Shows object hitboxes on death");
            setPriority(4);
        }
};