#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "PopupBase.hpp"

using namespace geode::prelude;

class OptionsUI : public PopupBase
{
    protected:
        Module* module = nullptr;
        BackgroundSprite* bg;
        CategoryNode* node;
        CCMenuItemToggler* favBtn = nullptr;

    public:
        static OptionsUI* create(Module* mod);

        void onInfo(CCObject* sender);
        void onInfoToggleFavourite(CCObject* sender);
        void onToggleFavourite(CCObject* sender);

        CCSize calculateSize();
        virtual bool setup();
};