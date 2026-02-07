#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/CategoryNode.hpp"
#include "PopupBase.hpp"

using namespace geode::prelude;

class ExtraThemeSettingsUI : public PopupBase
{
    protected:
        BackgroundSprite* bg;
        BackgroundSprite* imagePreview;
        bool image = false;

    public:
        static ExtraThemeSettingsUI* create(bool image = false);

        void onSelectImage(CCObject* sender);
        void onResetDefault(CCObject* sender);

        virtual bool setup();
};