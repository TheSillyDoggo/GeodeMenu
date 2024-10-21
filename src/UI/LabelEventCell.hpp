#pragma once

#include <Geode/Geode.hpp>
#include "../Layers/EditLabelPopup.hpp"

using namespace geode::prelude;

class LabelEventCell : public CCNode, public ColorPickPopupDelegate
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
        CCScale9Sprite* setColourBG;

        bool initWithEvent(LabelEvent* event);

        void updateSliders();
        void updateInputs();

        void onInf(CCObject* sender);
        void onSliderChanged(CCObject* sender);
        void onDelete(CCObject* sender);
        void onSetColour(CCObject* sender);

        virtual void updateColor(cocos2d::ccColor4B const& color);

        static LabelEventCell* createWithEvent(LabelEvent* event);
};