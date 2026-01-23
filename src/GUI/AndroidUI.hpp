#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "CategoryTabSprite.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "VersionInfoNode.hpp"

using namespace geode::prelude;

enum class MenuAnimation
{
    None,
    FromTop,
    FromBottom,
    FromLeft,
    FromRight,
    Scale,
    FadeIn
};

class AndroidUI : public geode::Popup<>
{
    protected:
        friend class ThemeNode;

        static inline AndroidUI* instance = nullptr;

        CCMenuItemSpriteExtra* backBtn = nullptr;
        BackgroundSprite* bg = nullptr;
        CCNode* categoryMenu;
        CCMenu* tabsMenu;
        CCNode* bottomTabsContainer = nullptr;
        Ref<CCRenderTexture> rt = nullptr;
        CCLayerColor* drawOpacity = nullptr;
        std::map<std::string, CategoryNode*> categories = {};
        std::map<std::string, CCMenuItemSpriteExtra*> categoryBtns = {};
        std::map<CCMenuItemSpriteExtra*, bool> categoryBtnsSelCheck = {};
        std::map<std::string, CategoryTabSprite*> categorySprs = {};
        // static to keep between reopens
        static inline std::string selectedCategory = "Level";
        VersionInfoNode* bottomLeft = nullptr;
        VersionInfoNode* bottomRight = nullptr;

        std::vector<std::string> categoryOrders =
        {
            "Level",
            "Universal",
            "Creator",
            "Cosmetic",
            "Speedhack",
            "Icon Effects",
            "Labels",
            "Config",
            "spacer",
            // "Profiles",
            "Search",
            "Favourites",
        };

        void keyDown(cocos2d::enumKeyCodes keym, double timestamp);

        ~AndroidUI();

    public:
        static AndroidUI* create();
        static AndroidUI* addToScene();
        static AndroidUI* get();

        void onSelectTab(CCObject* sender);

        void populateModules();
        void populateTabs();
        void updateTabs();

        void runAnimation(MenuAnimation anim);

        void addTab(std::string name, std::string id, std::string sprite);

        void close();
        virtual bool setup();
        virtual void visit();
        virtual void update(float dt);
};

class QOLModUIOpenEvent : public geode::Event
{
    public:
        AndroidUI* ui;

        QOLModUIOpenEvent(AndroidUI* ui)
        {
            this->ui = ui;
        }
};