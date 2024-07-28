#pragma once

#include "../Window.h"
#include "../idkwhattocallthis.hpp"

class Speedhack : public Window {
public:
    Speedhack() {
        name = "Speedhack";
        id = "speedhack-window";
    }

    static inline Slider* slider = nullptr;
    std::vector<float> presets = { 0.1f, 0.25f, 0.3f, 0.50f, 0.75f, 1, 1.50f, 2 };

    void clear(CCObject* sender);

    float scaleValue(float originalValue);

    float unscaleValue(float scaledValue);

    void sliderChanged(CCObject* sender);

    void onPreset(CCObject* sender);

    void cocosCreate(CCMenu* menu);

    virtual void textChanged(CCTextInputNode* p0);
};