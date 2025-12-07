#pragma once

#include "../../Client/FloatSliderModule.hpp"

using namespace geode::prelude;

class ButtonScale : public FloatSliderModule
{
    public:
        MODULE_SETUP(ButtonScale)
        {
            setName("Scale:");
            setID("button-scale");
            setCategory("Config/Button");
            setDefaultValue(0.85f);
            setSnapValues({ 0.85f });
            setRange(0.6f, 1.0f);
        }
};













SUBMIT_HACK(ButtonScale);