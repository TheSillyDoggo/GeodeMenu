#pragma once

#include "../Window.h"
#include "../idkwhattocallthis.hpp"

class Speedhack : public Window
{
    public:
        Speedhack()
        {
            name = "Speedhack";
            id = "speedhack-window";
        }

        Slider* slider = nullptr;
        TextInput* input;
        CCMenu* presetMenu;
        std::vector<CCMenuItemSpriteExtra*> presetBtns;
        std::vector<float> presets = { 0.1f, 0.25f, 0.3f, 0.50f, 0.75f, 1, 1.50f, 2 };
        bool isDeleting = false;
        ButtonSprite* sprD = nullptr;

        float scaleValue(float originalValue);
        float unscaleValue(float scaledValue);

        void sliderChanged(CCObject* sender);
        void onClear(CCObject* sender);
        void onPreset(CCObject* sender);
        void onNewPreset(CCObject* sender);
        void onDeletePreset(CCObject* sender);

        void cocosCreate(CCMenu* menu);

        void updatePresets();
        void updatePresetsSprites(int spr);

        virtual void textChanged(CCTextInputNode* p0);
};