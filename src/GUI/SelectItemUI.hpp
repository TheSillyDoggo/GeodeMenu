#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "BetterInputNode.hpp"

using namespace geode::prelude;

enum class SelectItemType
{
    ShortcutBG,
    ShortcutOverlay,
};

class SelectItemUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg;
        SelectItemType type;
        std::function<void(std::string)> onFinish = nullptr;
        CCMenu* buttonsMenu = nullptr;
        CCSprite* selectedOutline = nullptr;
        std::unordered_map<std::string, CCMenuItemSpriteExtra*> buttons = {};
        std::string selectedItem;

    public:
        static SelectItemUI* create(SelectItemType type, std::function<void(std::string)> onFinish);

        void onClose(CCObject* sender);
        void onSelectItem(CCObject* sender);

        void setSelectedItem(std::string item);
        void updateUI();

        virtual bool setup();
};