#pragma once

#include "Module.h"

class Hitboxes : public Module
{
    public:
        Hitboxes()
        {
            id = "show-hitboxes";
            name = "Show Hitboxes";
            description = "Shows object hitboxes";

            options.push_back(new ColourModule("Solid's Colour:", "colour-solid", ccc3(0, 0, 255)));
            options.push_back(new ColourModule("Hazards's Colour:", "colour-hazard", ccc3(255, 0, 0)));
            options.push_back(new ColourModule("Passable Colour:", "colour-passable", ccc3(0, 255, 255)));
            options.push_back(new ColourModule("Interactable:", "colour-interactable", ccc3(0, 255, 0)));
            options.push_back(new ColourModule("Player:", "colour-player", ccc3(255, 255, 0)));
            options.push_back(new ColourModule("Player (Rotated):", "colour-player-rot", ccc3(180, 170, 1)));

            options.push_back(new Module("Hitbox Trail", "show-hitbox-trail", "Creates a trail behind the player with it's hitbox"));
            options.push_back(new Module("Thicker Outline", "thick-hitbox", "Makes the outline around the hitboxes thicker"));

            options.push_back(new Module("Fill Hitbox", "fill-hitbox", "Fills the hitboxes as well as outlining them"));
            options.push_back(new SliderModule("Fill Opacity:", "fill-hitbox-opacity", 1.0f));


            this->load();
        }
};