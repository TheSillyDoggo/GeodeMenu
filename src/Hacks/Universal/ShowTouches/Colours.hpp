#pragma once

#include "../../../Client/ColourModule.hpp"

class CircleColour : public ColourModule
{
    public:
        MODULE_SETUP(CircleColour)
        {
            setName("Circle Colour:");
            setID("show-touches/circle-colour");

            setDefaultConfig({ cocos2d::ccc3(255, 255, 255) });
        }
};

class TrailColour : public ColourModule
{
    public:
        MODULE_SETUP(TrailColour)
        {
            setName("Trail Colour:");
            setID("show-touches/trail-colour");

            setDefaultConfig({ cocos2d::ccc3(255, 255, 255) });
        }
};