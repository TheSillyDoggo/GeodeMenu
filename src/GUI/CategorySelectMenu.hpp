#pragma once

#include <Geode/Geode.hpp>
#include "BetterScrollbar.hpp"
#include "CategoryTabSprite.hpp"

using namespace geode::prelude;

enum class CategoryType
{
    Standard,
    Extension,
    Bottom, // ts so me
};

class CategorySelectMenu : public CCMenu
{
    protected:
        float inset = 2.5f;
        ScrollLayer* scroll = nullptr;
        BetterScrollbar* scrollbar = nullptr;
        CCMenu* bottomBtns = nullptr;
        CCMenu* standardBtns = nullptr;
        std::vector<CCMenuItemSpriteExtra*> buttons = {};
        std::string selectedTab = "";
        std::function<void(std::string)> callback = nullptr;

        void onSelect(CCObject* sender);

        virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
        virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
        virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);

    public:
        CREATE_FUNC(CategorySelectMenu);

        void setInset(float inset);
        float getInset();

        void addCategory(std::string title, std::string sprite, std::string id, CategoryType type);
        void setSelectedCategory(std::string id);

        bool shouldScrollbarShow();
        void updateButtonSizes();

        CCMenuItemSpriteExtra* getButton(std::string id);

        void setCallback(std::function<void(std::string)> callback);

        virtual bool init();
        virtual void visit();
};