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

class AndroidUI : public PopupBase
{
    protected:
        friend class ThemeNode;

        static inline AndroidUI* instance = nullptr;

        CategorySelectMenu* catMenu = nullptr;
        CCMenuItemSpriteExtra* backBtn = nullptr;
        BackgroundSprite* bg = nullptr;
        CCNode* categoryMenu;
        CCNode* bottomTabsContainer = nullptr;
        Ref<CCRenderTexture> rt = nullptr;
        CCLayerColor* drawOpacity = nullptr;
        std::map<std::string, CategoryNode*> categories = {};
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
            // "Macros",
            "Labels",
            "Shortcuts",
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

        void populateModules();
        void populateTabs();
        void updateTabs();

        void runAnimation(MenuAnimation anim);

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