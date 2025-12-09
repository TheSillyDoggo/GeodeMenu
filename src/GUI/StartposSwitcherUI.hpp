#pragma once

#include <Geode/Geode.hpp>
#include "KeycodeNode.hpp"

using namespace geode::prelude;

class StartposSwitcherUI : public CCMenu
{
    protected:
        CCLabelBMFont* label = nullptr;
        CCLabelBMFont* leftPercent = nullptr;
        CCLabelBMFont* rightPercent = nullptr;
        CCSprite* leftSpr = nullptr;
        CCSprite* rightSpr = nullptr;
        KeycodeNode* leftKey = nullptr;
        KeycodeNode* rightKey = nullptr;
        int currentStartpos = 0;
    
    public:
        bool usePercentage = false;
        std::function<void(int)> onSwitchFunc = nullptr;

        static StartposSwitcherUI* create();

        void updateUI();
        void onSwitchBtn(CCObject* sender);
        void setStartposVisual(int index);
        void updateKeybindsVisualizer(bool visible, int leftCode, int rightCode);
        float getPercentForObject(GameObject* go);

        virtual bool init();
        virtual void updateDisplayedOpacity(GLubyte parentOpacity);
};