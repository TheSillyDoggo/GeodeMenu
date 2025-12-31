#include "BetterInputNode.hpp"
#include "../Utils/Num.hpp"
#include <Geode/cocos/extensions/GUI/CCEditBox/CCEditBoxImpl.h>

BetterInputNode* BetterInputNode::create(float width, std::string placeholder, std::string font)
{
    auto pRet = new BetterInputNode();

    if (pRet && pRet->init(width, placeholder, font))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool BetterInputNode::init(float width, std::string placeholder, std::string font)
{
    if (!CCMenu::init())
        return false;

    this->setContentSize(ccp(width, 30));
    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->ignoreAnchorPointForPosition(false);
    this->setTouchEnabled(true);
    this->setTouchPriority(-80085);

    ttfInput = CCTextFieldTTF::textFieldWithPlaceHolder("", "Arial.ttf", 16);
    ttfInput->setDelegate(this);
    ttfInput->setVisible(false);

    #ifdef BETTER_INPUT_NODE_USE_EDITBOX
    auto editBoxSpr = CCScale9Sprite::create("GJ_square01.png");

    editBoxInput = CCEditBox::create(ccp(100, 100), editBoxSpr);
    editBoxInput->setVisible(false);
    editBoxInput->setTouchEnabled(false);
    editBoxInput->setInputFlag(EditBoxInputFlag::kEditBoxInputFlagSensitive);
    editBoxInput->setInputMode(kEditBoxInputModeSingleLine);
    editBoxInput->setDelegate(this);
    this->addChild(editBoxInput);
    #endif

    bg = EasyBG::create();
    bg->setTargettingNode(this);
    bg->setAnchorPoint(ccp(0, 0));

    cursorCarot = CCLayerColor::create(ccc4(255, 255, 255, 125), 1.25f, 15);
    cursorCarot->ignoreAnchorPointForPosition(false);
    cursorCarot->setAnchorPoint(ccp(0.5f, 0.5f));

    labelContainer = CCNode::create();
    labelContainer->setAnchorPoint(ccp(0.5f, 0.5f));

    placeholderLbl = AdvLabelBMFont::createWithString(placeholder, font);
    placeholderLbl->setColor(ccc3(150, 150, 150));
    placeholderLbl->setAnchorPoint(ccp(0, 0));
    labelContainer->addChild(placeholderLbl);

    textLbl = AdvLabelBMFont::createWithStruct({}, font);
    textLbl->setAnchorPoint(ccp(0, 0));
    labelContainer->addChild(textLbl);

    this->addChild(bg);
    this->addChild(ttfInput);
    this->addChild(labelContainer);
    this->addChild(cursorCarot);
    return true;
}

void BetterInputNode::selectInput(bool selected)
{
    isSelected = selected;

    if (selected)
    {
        #ifdef BETTER_INPUT_NODE_USE_EDITBOX
        editBoxInput->m_pEditBoxImpl->openKeyboard();
        #else
        ttfInput->attachWithIME();
        #endif
        bg->getBG()->runAction(CCFadeTo::create(0.1f, 125));
    }
    else
    {
        #ifdef BETTER_INPUT_NODE_USE_EDITBOX
        editBoxInput->m_pEditBoxImpl->closeKeyboard();
        #else
        ttfInput->detachWithIME();
        #endif
        bg->getBG()->runAction(CCFadeTo::create(0.1f, 100));
    }
}

void BetterInputNode::visit()
{
    textLbl->setVisible(!getString().empty() || isSelected);
    placeholderLbl->setVisible(!textLbl->isVisible());

    labelContainer->setContentSize(textLbl->isVisible() ? textLbl->getContentSize() : placeholderLbl->getContentSize());
    labelContainer->setPositionY(getContentHeight() / 2);

    float scale = std::min((getContentHeight() - 10) / labelContainer->getContentHeight(), (getContentWidth() - 10) / labelContainer->getContentWidth());
    labelContainer->setScale(std::min(scale, 0.5f));

    switch (alignment)
    {
        case kCCTextAlignmentLeft:
            labelContainer->setPositionX(5);
            labelContainer->setAnchorPoint(ccp(0, 0.5f));
            break;

        case kCCTextAlignmentCenter:
            labelContainer->setPositionX(getContentWidth() / 2);
            labelContainer->setAnchorPoint(ccp(0.5f, 0.5f));
            break;

        case kCCTextAlignmentRight:
            labelContainer->setPositionX(getContentWidth() - 5);
            labelContainer->setAnchorPoint(ccp(1, 0.5f));
            break;
    }

    cursorCarot->setVisible(isSelected);
    cursorCarot->setPosition(ccp(labelContainer->getPositionX() + (labelContainer->getScaledContentWidth() * (1 - labelContainer->getAnchorPoint().x)), getContentHeight() / 2));

    CCMenu::visit();
}

void BetterInputNode::setString(std::string str)
{
    ttfInput->setString(str.c_str());
    textLbl->setString(str.c_str());

    #ifdef BETTER_INPUT_NODE_USE_EDITBOX
    editBoxInput->setText(str.c_str());
    #endif

    this->text = str;
}

std::string BetterInputNode::getString()
{
    return text;
}

void BetterInputNode::setAlignment(CCTextAlignment alignment)
{
    this->alignment = alignment;
}

CCTextAlignment BetterInputNode::getAlignment()
{
    return alignment;
}

bool BetterInputNode::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
    bg->getBG()->runAction(CCFadeTo::create(0.1f, 125));

    return false;
}

bool BetterInputNode::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    bg->getBG()->runAction(CCFadeTo::create(0.1f, 100));
    return false;
}

bool BetterInputNode::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen, cocos2d::enumKeyCodes code)
{
    if (code == enumKeyCodes::KEY_Unknown)
    {
        #if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)
        this->text.clear();
        #endif
        
        this->text.append(text);
        setString(this->text);
    }

    log::info("text: {}, len: {}, codes: {}", text, nLen, (int)code);
    
    return false;
}

bool BetterInputNode::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    setString(this->text.substr(0, this->text.size() - nLen));

    return false;
}

void BetterInputNode::editBoxEditingDidBegin(CCEditBox* editBox)
{
    onTextFieldAttachWithIME(nullptr);
}

void BetterInputNode::editBoxEditingDidEnd(CCEditBox* editBox)
{
    onTextFieldDetachWithIME(nullptr);
}

void BetterInputNode::editBoxTextChanged(CCEditBox* editBox, const gd::string& text)
{
    this->text = text;
    setString(this->text);
}

void BetterInputNode::editBoxReturn(CCEditBox* editBox)
{

}

bool BetterInputNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (getWorldSpaceBoundingBox(this).containsPoint(pTouch->getLocation()) && nodeIsVisible(this))
    {
        bg->getBG()->runAction(CCFadeTo::create(0.1f, 125));

        return true;
    }
    else
    {
        selectInput(false);

        return false;
    }
}

void BetterInputNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void BetterInputNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    selectInput(getWorldSpaceBoundingBox(this).containsPoint(pTouch->getLocation()));
}

void BetterInputNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}