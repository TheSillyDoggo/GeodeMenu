#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "PopupBase.hpp"
#include "ScrollLayer.hpp"

namespace qolmod
{
    class ItemListPopup : public PopupBase
    {
        protected:
            CCScale9Sprite* m_bg;
            AdvLabelBMFont* m_titleLabel;
            std::string m_title;
            std::string m_button;
            qolmod::ScrollLayer* m_scrollLayer;
            geode::Scrollbar* m_scrollBar;

            void updateSize();
            virtual bool setup();

        public:
            static ItemListPopup* create(char const* title, const char* btn);

            void addItem(const std::string& text);
            void addItem(const std::string& sprite, const std::string& text, bool frame = false);

            void onClose(CCObject* sender);
    };
};