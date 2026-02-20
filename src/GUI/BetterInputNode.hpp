#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "BetterInputNode.hpp"
#include "EasyBG.hpp"

using namespace geode::prelude;

class BetterInputNode : public CCMenu
{
    protected:
        std::string placeholder;
        std::string font;
        CCTextAlignment alignment = kCCTextAlignmentLeft;

        bool isSelected = false;
        std::string text = "";
        int cursorPos = -1;

        int maxChars = -1;
        std::string charFilter = "";

        bool isNumHoldActive = false;
        float numHoldStart = 0;

        bool numHoldEnabled = false;
        float numHoldStep = 1.0f;
        float numHoldInterval = 20;
        float numHoldDefault = 1.0f;

        static inline BetterInputNode* selectedInput = nullptr;
        TextInputDelegate* delegate = nullptr;

        geode::TextInput* textInput = nullptr;
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

        void updateCursorPos(bool isTouchUpdate, CCPoint touchPos);

        int getRealCursorPos();
        void moveCursor(int by);

        std::string filterString(std::string ss);
        ~BetterInputNode();

    public:
        static BetterInputNode* create(float width, std::string placeholder, std::string font = "bigFont.fnt");
        static BetterInputNode* getSelected();

        void selectInput(bool selected);

        void setString(std::string str);
        std::string getString();

        void setAlignment(CCTextAlignment alignment);
        CCTextAlignment getAlignment();

        void setDelegate(TextInputDelegate* delegate);
        TextInputDelegate* getDelegate();

        void setMaxChars(int max);
        void setCharFilter(std::string str);

        void setNumHoldValues(bool enabled, float step = 1.0f, float interval = 1.0f, float def = 1.0f);

        void handleKeypress(cocos2d::enumKeyCodes key, KeyboardInputData::Action action);

        void visit();
        bool init(float width, std::string placeholder, std::string font = "bigFont.fnt");
};