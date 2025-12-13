#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/CategoryNode.hpp"

using namespace geode::prelude;

class ExtraThemeSettingsUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg;

    public:
        static ExtraThemeSettingsUI* create();

        virtual bool setup();
};