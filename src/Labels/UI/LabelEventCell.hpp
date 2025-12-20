#pragma once

#include <Geode/Geode.hpp>
#include "../LabelConfig.hpp"
#include "SetupLabelConfigUI.hpp"
#include "../../GUI/BetterSlider.hpp"

using namespace geode::prelude;

class LabelEventCell : public CCNode
{
    protected:
        CCSize size;
        LabelEvent config;
        int index;
        SetupLabelConfigUI* ui;
        BetterSlider* sliderFadeIn;
        BetterSlider* sliderHold;
        BetterSlider* sliderFadeOut;
        TextInput* inputFadeIn;
        TextInput* inputHold;
        TextInput* inputFadeOut;

    public:
        static LabelEventCell* create(CCSize size, LabelEvent config, SetupLabelConfigUI* ui, int index);

        void onSetColour(CCObject* sender);
        void onDelete(CCObject* sender);
        void onInf(CCObject* sender);
        void onSliderChanged(CCObject* sender);

        bool init(CCSize size, LabelEvent config, SetupLabelConfigUI* ui, int index);
};