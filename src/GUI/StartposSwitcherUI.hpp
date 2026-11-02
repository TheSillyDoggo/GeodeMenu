#pragma once

#include <Geode/Geode.hpp>
#include "KeycodeNode.hpp"

using namespace geode::prelude;

class StartposSwitcherUI : public CCMenu, public cocos2d::CCActionTweenDelegate
{
    protected:
        CCLabelBMFont* label = nullptr;
        CCLabelBMFont* nameLabel = nullptr;
        CCLabelBMFont* leftPercent = nullptr;
        CCLabelBMFont* rightPercent = nullptr;
        CCSprite* leftSpr = nullptr;
        CCSprite* rightSpr = nullptr;

        GJBaseGameLayer* gameLayer;
        bool updatingEnabled = true;

        float opacityMode = 1.0f;

        virtual void updateTweenAction(float value, const char* key);
    
    public:
        bool usePercentage = false;
        bool showName = true;
        float regularOpacity = 0.2f;

        static StartposSwitcherUI* create(GJBaseGameLayer* gameLayer, bool updatingEnabled = true);

        void updateUI();
        void onSwitchBtn(CCObject* sender);
        float getPercentForObject(GameObject* go);

        void runOpacityAction();

        virtual bool init();
        virtual void updateDisplayedOpacity(GLubyte parentOpacity);
};