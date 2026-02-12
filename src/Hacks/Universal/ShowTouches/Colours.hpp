#pragma once

#include "../../../Client/ColourModule.hpp"
#include <InputModule.hpp>

class CircleColour : public ColourModule
{
    public:
        MODULE_SETUP(CircleColour)
        {
            setID("show-touches/circle-colour");
            setPriority(1);

            setDefaultConfig({ cocos2d::ccc3(255, 255, 255) });
        }
};

class TrailColour : public ColourModule
{
    public:
        MODULE_SETUP(TrailColour)
        {
            setID("show-touches/trail-colour");
            setPriority(2);

            setDefaultConfig({ cocos2d::ccc3(255, 255, 255) });
        }
};

class CircleScale : public InputModule
{
    public:
        MODULE_SETUP(CircleScale)
        {
            setID("show-touches/scale");

            setDefaultString("1.00");
            setPriority(3);

            setStringFilter("1234567890.");
            setMaxCharCount(4);
        }
};

class TrailEnabled : public Module
{
    public:
        MODULE_SETUP(TrailEnabled)
        {
            setID("show-touches/use-trail");
            setDefaultEnabled(true);
            setPriority(4);
        }
};

class ShowTouchDuration : public Module
{
    public:
        MODULE_SETUP(ShowTouchDuration)
        {
            setID("show-touches/show-duration");
            setDefaultEnabled(false);
            setPriority(5);
        }
};