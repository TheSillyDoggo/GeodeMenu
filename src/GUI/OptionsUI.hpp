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
        static inline OptionsUI* instance = nullptr;
        Module* module = nullptr;
        qolmod::BackgroundSprite* bg;
        CategoryNode* node;
        CCMenuItemToggler* favBtn = nullptr;

        ~OptionsUI();

    public:
        static OptionsUI* create(Module* mod);

        void onInfo(CCObject* sender);
        void onToggleFavourite(CCObject* sender);
        void onChangeShortcut(CCObject* sender);
        void onChangeKeybind(CCObject* sender);

        CCSize calculateSize();
        virtual bool setup();
};