#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"

using namespace geode::prelude;

class SliderModule : public Module
{
    public:
        Slider* slider = nullptr;
        float value = 1.0f;

        SliderModule(std::string name, std::string id, float def);

        void save();
        void load();

        void onSliderChanged(CCObject* sender);

        void makeAndroid(CCNode* menu, CCPoint pos);
};