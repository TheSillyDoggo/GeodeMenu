#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "BetterInputNode.hpp"
#include "EasyBG.hpp"

using namespace geode::prelude;

class BetterInputNode : public CCMenu, public CCTextFieldDelegate
{
    protected:
        std::string placeholder;
        std::string font;
        CCTextAlignment alignment = kCCTextAlignmentCenter;

        bool isSelected = false;
        std::string text = "";
        int cursorPos = -1;

        CCLayerColor* cursorCarot = nullptr;
        CCTextFieldTTF* ttfInput = nullptr;
        EasyBG* bg = nullptr;
        AdvLabelBMFont* placeholderLbl = nullptr;
        AdvLabelBMFont* textLbl = nullptr;
        CCNode* labelContainer = nullptr;
        CCLabelTTF* textLblUser = nullptr;

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

        virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
        virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
        virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char* text, int nLen, cocos2d::enumKeyCodes);
        virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char* delText, int nLen);

    public:
        static BetterInputNode* create(float width, std::string placeholder, std::string font = "bigFont.fnt");

        void selectInput(bool selected);

        void setString(std::string str);
        std::string getString();

        void setAlignment(CCTextAlignment alignment);
        CCTextAlignment getAlignment();

        void visit();
        bool init(float width, std::string placeholder, std::string font = "bigFont.fnt");
};