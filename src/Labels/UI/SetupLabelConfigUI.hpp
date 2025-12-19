#pragma once

#include <Geode/Geode.hpp>
#include "../../GUI/BackgroundSprite.hpp"
#include "../LabelConfig.hpp"

using namespace geode::prelude;

class SetupLabelConfigUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg = nullptr;
        std::function<void(LabelConfig)> onFinish = nullptr;
        LabelConfig startConfig;
        LabelConfig currentConfig;
        CCMenu* pagesMenu = nullptr;
        CCMenu* anchorMenu = nullptr;
        std::unordered_map<LabelAnchor, CCMenuItemToggler*> anchorBtns = {};
        std::vector<CCNode*> pages = {};
        std::vector<CCMenuItemToggler*> pageBtns = {};
        int selectedPage = 0;

    public:
        static SetupLabelConfigUI* create(std::function<void(LabelConfig)> onFinish);

        void onSetAnchor(CCObject* sender);
        void onClose(CCObject* sender);
        void onChangePage(CCObject* sender);

        void createPages();
        void createAnchorNodes();

        void setStartConfig(LabelConfig conf);
        void updateUI();

        virtual bool setup();
};