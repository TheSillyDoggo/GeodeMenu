#pragma once

#include "../../../Client/ColourModule.hpp"
#include "../../../Client/InputModule.hpp"
#include "../../../Client/FloatSliderModule.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class HitboxSolid : public ColourModule
{
    public:
        MODULE_SETUP(HitboxSolid)
        {
            setName("Solids:");
            setID("show-hitboxes/solid-colour");
            setDefaultConfig({ ccc3(0, 0, 255) });
            setPriority(1);
        }
};

class HitboxHazard : public ColourModule
{
    public:
        MODULE_SETUP(HitboxHazard)
        {
            setName("Hazards:");
            setID("show-hitboxes/hazard-colour");
            setDefaultConfig({ ccc3(255, 0, 0) });
            setPriority(2);
        }
};

class HitboxPassable : public ColourModule
{
    public:
        MODULE_SETUP(HitboxPassable)
        {
            setName("Passable:");
            setID("show-hitboxes/passable-colour");
            setDefaultConfig({ ccc3(0, 255, 255) });
            setPriority(3);
        }
};

class HitboxInteractable : public ColourModule
{
    public:
        MODULE_SETUP(HitboxInteractable)
        {
            setName("Interactable:");
            setID("show-hitboxes/interactable-colour");
            setDefaultConfig({ ccc3(0, 255, 0) });
            setPriority(4);
        }
};

class HitboxPlayer : public ColourModule
{
    public:
        MODULE_SETUP(HitboxPlayer)
        {
            setName("Player:");
            setID("show-hitboxes/player-colour");
            setDefaultConfig({ ccc3(255, 0, 0) });
            setPriority(5);
        }
};

class HitboxPlayerRot : public ColourModule
{
    public:
        MODULE_SETUP(HitboxPlayerRot)
        {
            setName("Player (rotated):");
            setID("show-hitboxes/player-rotated-colour");
            setDefaultConfig({ ccc3(170, 0, 0) });
            setPriority(6);
        }
};

class HitboxThickOutline : public Module
{
    public:
        MODULE_SETUP(HitboxThickOutline)
        {
            setName("Thick Outline");
            setID("show-hitboxes/thick-hitboxes");
            setDescription("Makes the outline 2x thicker");
            setPriority(7);
        }
};

class HitboxFill : public Module
{
    public:
        MODULE_SETUP(HitboxFill)
        {
            setName("Fill hitboxes");
            setID("show-hitboxes/fill-hitboxes");
            setDescription("Fills between the outlines");
            setPriority(9);
        }
};

class HitboxFillOpacity : public FloatSliderModule
{
    public:
        MODULE_SETUP(HitboxFillOpacity)
        {
            setName("Fill Opacity:");
            setID("show-hitboxes/fill-opacity");
            setPriority(10);

            setRange(0, 1);
            setDefaultValue(1.0f);
        }
};