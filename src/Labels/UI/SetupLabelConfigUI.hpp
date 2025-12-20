#pragma once

#include <Geode/Geode.hpp>
#include "../../GUI/BackgroundSprite.hpp"
#include "../LabelConfig.hpp"

using namespace geode::prelude;

class SetupLabelConfigUI : public geode::Popup<>
{
    friend class LabelEventCell;

    protected:
        BackgroundSprite* bg = nullptr;
        std::function<void(LabelConfig)> onFinish = nullptr;
        LabelConfig startConfig;
        LabelConfig currentConfig;
        CCMenu* pagesMenu = nullptr;
        CCMenu* anchorMenu = nullptr;
        std::unordered_map<LabelAnchor, CCMenuItemToggler*> anchorBtns = {};
        TextInput* formatInp;
        TextInput* nameInp;
        TextInput* scaleInp;
        TextInput* opacityInp;
        CCMenuItemToggler* cheatIndicatorToggler;
        CCMenuItemToggler* noclipOnlyToggler;
        TextInput* offsetXInp;
        TextInput* offsetYInp;
        ScrollLayer* eventScroll;
        CCLabelBMFont* eventsNone;
        std::vector<CCNode*> pages = {};
        std::vector<CCMenuItemToggler*> pageBtns = {};
        int selectedPage = 0;

    public:
        static SetupLabelConfigUI* create(std::function<void(LabelConfig)> onFinish);

        void onSetAnchor(CCObject* sender);
        void onAddEvent(CCObject* sender);
        void onClose(CCObject* sender);
        void onChangePage(CCObject* sender);

        void createPages();
        void createAnchorNodes();
        void createPage1();
        void createPage2();
        void createPage3();

        void setStartConfig(LabelConfig conf);
        void updateUI();
        void updateEventsUI();

        virtual bool setup();
};