#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct KeycodeListenerLayerSettings
{
    int currentKeycode = -1;
    int defaultKeycode = -1;
    bool allowCancel = true;
    bool allowDelete = true;
    bool allowShiftKeys = false;
    bool allowModifiers = true;
};

class KeycodeListenerLayer : public geode::Popup<>
{
    protected:
        KeycodeListenerLayerSettings settings;
        std::function<void(int)> onSucceed = nullptr;

    public:
        static KeycodeListenerLayer* create(KeycodeListenerLayerSettings settings, std::function<void(int)> onSucceed);

        void onCloseBtn(CCObject* sender);

        virtual void keyDown(cocos2d::enumKeyCodes key);
        virtual void keyBackClicked();
        virtual bool setup();
};