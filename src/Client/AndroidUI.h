#pragma once

#include <Geode/Geode.hpp>
#include "ClientSetup.h"
#include "../Utils/defines.hpp"

using namespace geode::prelude;

class AndroidUI : public geode::Popup<>, TextInputDelegate
{
    public:
        std::vector<CCLabelBMFont*> labels = {};
        std::vector<CCScale9Sprite*> outlines = {};
        std::vector<CCMenuItemSpriteExtra*> buttons = {};
        std::vector<CCMenu*> pages = {};
        CCScale9Sprite* panel = nullptr;
        CCLabelBMFont* searchLabel = nullptr;
        CCMenu* searchResultsPanel = nullptr;
        CCMenu* btnMenu = nullptr;
        TextInput* inputField = nullptr;
        ScrollLayer* scroll = nullptr;

        static inline int lastTab = 0;
        static inline int selectedTab = 0;

        void goToPage(int p, bool transition = false);
        
        void onClose(CCObject* sender);
        void onPressTab(CCObject* sender);

        virtual bool setup();
        virtual void textChanged(CCTextInputNode* p0);
        virtual void update(float dt);
        virtual void keyBackClicked();

        CCAction* getEnterAction(CCNode* panel);
        CCNode* getBGNode();
        CCMenu* getSearchPanel();

        static AndroidUI* addToScene();
        static AndroidUI* create();

        ~AndroidUI();
};