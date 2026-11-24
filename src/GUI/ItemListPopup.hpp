#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "PopupBase.hpp"
#include "ScrollLayer.hpp"
#include "Button.hpp"

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
            cocos2d::CCMenu* m_contentMenu;
            std::unordered_map<qolmod::Button*, std::function<void()>> m_callbacks;

            void selectColour(cocos2d::CCNode* container);
            void deselectColour(cocos2d::CCNode* container);

            void onItemClicked(cocos2d::CCObject* sender);
            void updateSize();
            virtual bool setup();

        public:
            static ItemListPopup* create(char const* title, const char* btn);

            void addItem(const std::string& text);
            void addItem(const std::string& sprite, const std::string& text, bool frame = false, std::function<void()> callback = nullptr);

            void onClose(cocos2d::CCObject* sender);
    };
};