#pragma once

#include "../../../Client/Module.hpp"
#include "../../../Client/InputModule.hpp"
#include <ColourModule.hpp>

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

        virtual void onToggle();
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

class ShowHitboxesOnDeathDeathObjOnly : public Module
{
    public:
        MODULE_SETUP(ShowHitboxesOnDeathDeathObjOnly)
        {
            setID("show-hitboxes-on-death/death-object-only");
            setPriority(1);
        }
};

class ShowHitboxesOnDeathTrail : public Module
{
    public:
        MODULE_SETUP(ShowHitboxesOnDeathTrail)
        {
            setID("show-hitboxes-on-death/trail");
            setDefaultEnabled(true);
            setPriority(2);
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
            setPriority(1);

            setDefaultString("480");
            setPlaceholderString("Positions");

            setStringFilter("1234567890");
            setMaxCharCount(5);
        }
};

class HitboxTrailResetOnDeath : public Module
{
    public:
        MODULE_SETUP(HitboxTrailResetOnDeath)
        {
            setName("Reset positions on death");
            setID("hitbox-trail/reset-positions-on-death");
            setDescription("Resets the stored positions every attempt");
            setDefaultEnabled(true);
            setPriority(2);
        }
};

class HitboxTrailDarkenByAge : public Module
{
    public:
        MODULE_SETUP(HitboxTrailDarkenByAge)
        {
            setID("hitbox-trail/darken-by-age");
            setDefaultEnabled(true);
            setPriority(3);
        }
};

class HitboxTrailDoClickColours : public Module
{
    public:
        MODULE_SETUP(HitboxTrailDoClickColours)
        {
            setID("hitbox-trail/click-colours");
            setDefaultEnabled(true);
            setPriority(4);
        }
};

class HitboxTrailStartClickCol : public ColourModule
{
    public:
        MODULE_SETUP(HitboxTrailStartClickCol)
        {
            setID("hitbox-trail/start-click-colour");
            setDefaultConfig({ ccc3(0, 255, 0) });
            setPriority(5);
        }
};

class HitboxTrailEndClickCol : public ColourModule
{
    public:
        MODULE_SETUP(HitboxTrailEndClickCol)
        {
            setID("hitbox-trail/end-click-colour");
            setDefaultConfig({ ccc3(0, 255, 255) });
            setPriority(6);
        }
};

class HitboxTrailHoldClickColours : public Module
{
    public:
        MODULE_SETUP(HitboxTrailHoldClickColours)
        {
            setID("hitbox-trail/hold-click-colours");
            setDefaultEnabled(false);
            setPriority(7);
        }
};

class HitboxTrailMidClickCol : public ColourModule
{
    public:
        MODULE_SETUP(HitboxTrailMidClickCol)
        {
            setID("hitbox-trail/hold-click-colour");
            setDefaultConfig({ ccc3(0, 210, 0) });
            setPriority(8);
        }
};