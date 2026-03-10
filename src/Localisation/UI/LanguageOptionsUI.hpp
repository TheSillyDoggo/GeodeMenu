#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "PopupBase.hpp"

using namespace geode::prelude;

class LanguageOptionsUI : public PopupBase
{
    protected:
        BackgroundSprite* bg;
        CategoryNode* node;
        static inline LanguageOptionsUI* instance = nullptr;

        ~LanguageOptionsUI();

    public:
        static LanguageOptionsUI* create();
        static LanguageOptionsUI* get();

        virtual bool setup();
};