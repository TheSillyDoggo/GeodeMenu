#pragma once

#include "../../Client/FloatSliderModule.hpp"

using namespace geode::prelude;

class PopupUIScale : public FloatSliderModule
{
    public:
        MODULE_SETUP(PopupUIScale)
        {
            setID("popup-ui-scale");
            setCategory("Config/Menu");
            setDefaultValue(1);
            setRange(0.6f, 1.0f);
        }
};