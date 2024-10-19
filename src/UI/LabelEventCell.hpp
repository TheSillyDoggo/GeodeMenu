#pragma once

#include <Geode/Geode.hpp>
#include "../Layers/EditLabelPopup.hpp"

using namespace geode::prelude;

class LabelEventCell : public CCNode
{
    public:
        LabelEvent* event;
        LabelModule* module;
        CCLayer* layer;
        Slider* sliderFadeIn;
        Slider* sliderHold;
        Slider* sliderFadeOut;
        TextInput* inputFadeIn;
        TextInput* inputHold;
        TextInput* inputFadeOut;

        bool initWithEvent(LabelEvent* event);

        void onSliderChanged(CCObject* sender);
        void onDelete(CCObject* sender);

        static LabelEventCell* createWithEvent(LabelEvent* event);
};