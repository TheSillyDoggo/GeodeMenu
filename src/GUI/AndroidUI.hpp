#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"
#include "CategoryTabSprite.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "VersionInfoNode.hpp"
#include "PopupBase.hpp"
#include "CategorySelectMenu.hpp"
#include <Button.hpp>
#include <AltMouseDelegate.hpp>

using namespace geode::prelude;

class AndroidUI : public PopupBase, public qolmod::AltMouseDelegate
{
    protected:
        friend class ThemeNode;
        friend class ExtraThemeSettingsUI;

        static inline AndroidUI* instance = nullptr;

        CategorySelectMenu* catMenu = nullptr;
        qolmod::Button* backBtn = nullptr;
        qolmod::BackgroundSprite* bg = nullptr;
        CCNode* categoryMenu;
        CCNode* bottomTabsContainer = nullptr;
        std::map<std::string, CategoryNode*> categories = {};
        // static to keep between reopens
        static inline std::string selectedCategory = "Level";
        std::vector<std::string> categoryHistory = {};
        VersionInfoNode* bottomLeft = nullptr;
        VersionInfoNode* bottomRight = nullptr;
        bool allow = false;

        std::vector<std::string> categoryOrders =
        {
            "Level",
            "Universal",
            "Creator",
            "Cosmetic",
            "Speedhack",
            "Icon Effects",
            // "Macros",
            "Labels",
            "Shortcuts",
            "Config",
            "spacer",
            // "Profiles",
            "Search",
            "Favourites",
        };

        virtual void keyDown(cocos2d::enumKeyCodes keym, double timestamp);
        virtual bool altClickBegan(int button, cocos2d::CCPoint point);

        ~AndroidUI();

    public:
        static AndroidUI* create();
        static AndroidUI* addToScene();
        static AndroidUI* get();

        void populateModules();
        void populateTabs();
        void updateTabs();

        void switchTabTemp(std::string tab);

        void close();
        virtual bool setup();
        virtual void visit();
};

/*class QOLModUIOpenEvent : public geode::Event
{
    public:
        AndroidUI* ui;

        QOLModUIOpenEvent(AndroidUI* ui)
        {
            this->ui = ui;
        }
};*/