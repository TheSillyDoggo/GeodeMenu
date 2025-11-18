#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class StartposSwitcherUI : public CCMenu
{
    protected:
        CCLabelBMFont* label;
        CCSprite* leftSpr;
        CCSprite* rightSpr;
        int currentStartpos = 0;
    
    public:
        std::function<void(int)> onSwitchFunc = nullptr;

        static StartposSwitcherUI* create();

        void updateUI();
        void onSwitchBtn(CCObject* sender);
        void setStartposVisual(int index);

        virtual bool init();
        virtual void updateDisplayedOpacity(GLubyte parentOpacity);
};