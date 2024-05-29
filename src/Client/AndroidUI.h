#pragma once

#include <Geode/Geode.hpp>
#include "ClientSetup.h"
#include "../Utils/defines.hpp"

using namespace geode::prelude;

class AndroidUI : public cocos2d::CCLayerColor, TextInputDelegate {
public:
    static inline std::vector<CCLabelBMFont*> labels = {};
    static inline std::vector<CCScale9Sprite*> outlines = {};
    static inline std::vector<CCMenuItemSpriteExtra*> buttons = {};
    static inline std::vector<CCMenu*> pages = {};
    CCScale9Sprite* panel = nullptr;
    CCLabelBMFont* searchLabel = nullptr;
    CCMenu* searchResultsPanel = nullptr;
    CCMenu* btnMenu = nullptr;
    TextInput* inputField = nullptr;
    ScrollLayer* scroll = nullptr;

    static inline int lastTab = 0;
    static inline int selectedTab = 0;

    int secret = 0;
    
    void goToPage(int p, bool transition = false);

    virtual void keyBackClicked();

    void close(CCObject* sender);

    CCAction* getEnterAction(CCNode* panel);

    void onPressTab(CCObject* sender);

    virtual bool init();

    virtual void update(float dt);

    virtual void textChanged(CCTextInputNode* p0);

    float roundUpToMultipleOf2(float num) { // def not stolen
        float roundedNum = std::ceil(num / 2.0f) * 2.0f;
        return roundedNum;
    }

    CCMenu* getSearchPanel();

    CREATE_FUNC(AndroidUI);

    static void addToScene()
    {
        if (CCDirector::get()->getRunningScene()->getChildByID("AndroidUI"))
            return;

        auto l = AndroidUI::create();

        CCDirector::get()->getRunningScene()->addChild(l, 69420);
    }

    ~AndroidUI();
};