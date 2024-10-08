#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Module.h"
#include "../Hacks/SafeMode/SafeMode.hpp"
#include "../Labels/LabelCommon.hpp"
#include "SillyBaseLayer.h"

using namespace geode::prelude;

class EditSafeZonePopup : public SillyBaseLayer
{
    public:
        CCNode* bottomLeft;
        CCNode* bottomRight;
        CCNode* topLeft;
        CCNode* topRight;
        Slider* topSlider;
        Slider* bottomSlider;
        Slider* leftSlider;
        Slider* rightSlider;

        virtual void customSetup();
        virtual void update(float dt);

        void onSliderChanged(CCObject* sender);

        static EditSafeZonePopup* create();
        static EditSafeZonePopup* addToScene();
};