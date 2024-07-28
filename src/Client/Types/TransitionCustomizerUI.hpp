#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"

using namespace geode::prelude;

class TransCustomizerModule : public Module
{
    public:
        static inline CCLabelBMFont* label = nullptr;
        static inline CCMenuItemSpriteExtra* left = nullptr;
        static inline CCMenuItemSpriteExtra* right = nullptr;

        static inline const std::vector<std::string> transNames = {"Fade", "Cross Fade", "Fade Bottom Left", "Fade Top Right", "Fade Up", "Fade Down", "Flip Angular", "Flip X", "Flip Y", "Zoom Flip Angular", "Zoom Flip X", "Zoom Flip Y", "Jump Zoom", "Move In Top", "Move In Bottom", "Move In Left", "Move In Right", "Rotate Zoom", "Shrink Grow", "Slide In Top", "Slide In Bottom", "Slide In Left", "Slide In Right", "Split Rows", "Split Columns", "Tiles"};

        TransCustomizerModule();

        void save();
        void load();

        void makeAndroid(CCNode* menu, CCPoint pos);

        void onLeft(CCObject*);
        void onRight(CCObject*);
};