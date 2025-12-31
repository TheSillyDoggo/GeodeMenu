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
        CCTextAlignment alignment = kCCTextAlignmentLeft;

        bool isSelected = false;
        std::string text = "";
        int cursorPos = -1;

        bool isNumHoldActive = false;
        float numHoldStart = 0;

        bool numHoldEnabled = false;
        float numHoldStep = 1.0f;
        float numHoldInterval = 20;
        float numHoldDefault = 1.0f;

        TextInputDelegate* delegate = nullptr;

        CCTextFieldTTF* ttfInput = nullptr;
        CCLayerColor* cursorCarot = nullptr;
        EasyBG* bg = nullptr;
        AdvLabelBMFont* placeholderLbl = nullptr;
        AdvLabelBMFont* textLbl = nullptr;
        AdvLabelBMFont* hintLbl = nullptr;
        CCLabelTTF* textLblUser = nullptr;
        bool useTTFView = false;
        CCNode* labelContainer = nullptr;

        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
        virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

        virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
        virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
        virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char* text, int nLen, cocos2d::enumKeyCodes);
        virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char* delText, int nLen);
        virtual bool onDraw(CCTextFieldTTF * sender);

        int getRealCursorPos();
        void moveCursor(int by);

    public:
        static BetterInputNode* create(float width, std::string placeholder, std::string font = "bigFont.fnt");

        void selectInput(bool selected);

        void setString(std::string str);
        std::string getString();

        void setAlignment(CCTextAlignment alignment);
        CCTextAlignment getAlignment();

        void setDelegate(TextInputDelegate* delegate);
        TextInputDelegate* getDelegate();

        void setNumHoldValues(bool enabled, float step = 1.0f, float interval = 1.0f, float def = 1.0f);

        void visit();
        bool init(float width, std::string placeholder, std::string font = "bigFont.fnt");
};