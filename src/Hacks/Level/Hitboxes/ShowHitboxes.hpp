#pragma once

#include "../../../Client/Module.hpp"
#include "../../../Client/InputModule.hpp"

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

class HitboxTrail : public Module
{
    public:
        MODULE_SETUP(HitboxTrail)
        {
            setName("Hitbox Trail");
            setID("hitbox-trail");
            setCategory("Level");
            setDescription("Shows the trail of the player");
            setPriority(5);
        }
};

class HitboxTrailMaxPositions : public InputModule
{
    public:
        MODULE_SETUP(HitboxTrailMaxPositions)
        {
            setName("Stored positions");
            setID("hitbox-trail/stored-positions");
            setDescription("");

            setDefaultString("480");
            setPlaceholderString("Positions");

            setStringFilter("1234567890");
            setMaxCharCount(5);
        }
};