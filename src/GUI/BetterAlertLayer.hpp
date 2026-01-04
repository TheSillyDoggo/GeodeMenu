#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

class BetterAlertLayer : public geode::Popup<>
{
    protected:
        CCScale9Sprite* bg = nullptr;
        AdvLabelBMFont* titleLabel = nullptr;
        AdvLabelBMFont* content = nullptr;

        FLAlertLayerProtocol* delegate;
        char const* title;
        std::string desc;
        char const* btn1;
        char const* btn2;
        float width;
        bool scroll;
        float height;
        float textScale;

    public:
        static BetterAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale);
        static BetterAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width);
        static BetterAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2);
        static BetterAlertLayer* create(char const* title, const gd::string& desc, char const* btn);
        static BetterAlertLayer* createWithLocalisation(char const* title, const gd::string& desc, char const* btn);

        void onButton(CCObject* sender);

        virtual bool setup();
};