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

    public:
        static ExtraThemeSettingsUI* create();

        virtual bool setup();
};