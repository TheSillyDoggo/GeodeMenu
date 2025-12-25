#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "BetterScrollbar.hpp"

using namespace geode::prelude;

class SelectFontUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg;
        std::string defaultFont = "bigFont.fnt";
        std::string startFont = "";
        std::string currentFont = "";
        bool allowNonGDFonts = true;
        std::function<void(std::string)> onFinish = nullptr;
        std::unordered_map<std::string, AdvLabelBMFont*> labels = {};
        std::unordered_map<std::string, CCMenuItemToggler*> togglers = {};
        ScrollLayer* scroll1 = nullptr;
        ScrollLayer* scroll2 = nullptr;
        BetterScrollbar* scrollbar1 = nullptr;
        BetterScrollbar* scrollbar2 = nullptr;
        bool page2 = false;

    public:
        static SelectFontUI* create(std::function<void(std::string)> onFinish, bool allowNonGDFonts = true);

        void setDefaultFont(std::string font);
        void setStartFont(std::string font);
        void updateUI(CCMenuItemToggler* except = nullptr);

        void onFontHint(CCObject* sender);
        void onSetDefault(CCObject* sender);
        void onUndoChanged(CCObject* sender);
        void onChangeFont(CCObject* sender);
        void onChangePage(CCObject* sender);
        void onClose(CCObject* sender);

        CCMenu* createFontCell(std::string font, int index, std::string name);
        CCMenu* getHints(CCBMFontConfiguration* config);

        virtual void update(float dt);
        virtual bool setup();
};