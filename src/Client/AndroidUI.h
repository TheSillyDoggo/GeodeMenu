#pragma once

#include <Geode/Geode.hpp>
#include "ClientSetup.h"

using namespace geode::prelude;

class AndroidUI : public cocos2d::CCLayerColor {
public:
    static inline std::vector<CCLabelBMFont*> labels = {};
    static inline std::vector<CCMenuItemSprite*> buttons = {};
    static inline std::vector<CCMenu*> pages = {};
    CCScale9Sprite* panel = nullptr;

    static inline int lastTab = 0;
    static inline int selectedTab = 0;

    int secret = 0;
    
    void goToPage(int p, bool transition = false);

    virtual void keyBackClicked();

    void close(CCObject* sender);

    CCAction* getEnterAction(CCNode* panel);

    void onPressTab(CCObject* sender);

    virtual bool init();

    CREATE_FUNC(AndroidUI);

    static void addToScene()
    {
        if (CCDirector::get()->getRunningScene()->getChildByID("android-ui"))
            return;

        auto l = AndroidUI::create();

        CCDirector::get()->getRunningScene()->addChild(l, 69420);
    }
};