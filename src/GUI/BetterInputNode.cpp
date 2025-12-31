#include "BetterInputNode.hpp"
#include "../Utils/Num.hpp"

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

    textLblUser = CCLabelTTF::create("", "Arial.ttf", 32);
    textLblUser->setAnchorPoint(ccp(0, 0));

    labelContainer->addChild(textLbl);
    labelContainer->addChild(textLblUser);

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
        ttfInput->attachWithIME();
        bg->getBG()->runAction(CCFadeTo::create(0.1f, 125));
    }
    else
    {
        ttfInput->detachWithIME();
        bg->getBG()->runAction(CCFadeTo::create(0.1f, 100));
    }
}

void BetterInputNode::visit()
{
    textLbl->setVisible(!getString().empty() || isSelected);
    placeholderLbl->setVisible(!textLbl->isVisible());

    labelContainer->setContentSize(textLbl->isVisible() ? (useTTFView ? textLblUser->getContentSize() : textLbl->getContentSize()) : placeholderLbl->getContentSize());
    labelContainer->setPositionY(getContentHeight() / 2);

    textLblUser->setVisible(textLbl->isVisible() && useTTFView);
    textLbl->setVisible(textLbl->isVisible() && !useTTFView);

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
    ttfInput->m_uCursorPos = str.size() + 1;

    ttfInput->setString(str.c_str());
    textLbl->setString(str.c_str());

    useTTFView = textLbl->doesLabelContainNonDisplayableCharacter();

    if (useTTFView)
    {
        textLblUser->setString(str.c_str());
    }

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

        if (delegate)
            delegate->textChanged(nullptr);
    }
    
    return false;
}

bool BetterInputNode::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    setString(this->text.substr(0, this->text.size() - nLen));

    if (delegate)
        delegate->textChanged(nullptr);

    return false;
}

void BetterInputNode::setNumHoldValues(bool enabled, float step, float interval, float def)
{
    this->numHoldEnabled = enabled;
    this->numHoldStep = step;
    this->numHoldInterval = interval;
    this->numHoldDefault = def;
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
    if (!isNumHoldActive && numHoldEnabled && std::abs<float>(pTouch->getStartLocation().y - pTouch->getLocation().y) > numHoldInterval)
    {
        numHoldStart = utils::numFromString<float>(text).unwrapOr(numHoldDefault);
        isNumHoldActive = true;
    }

    if (isNumHoldActive)
    {
        int steps = (pTouch->getLocation().y - pTouch->getStartLocation().y) / numHoldInterval;

        float v = numHoldStart + (numHoldStep * steps);

        setString(utils::numToString<float>(v, 2));

        if (delegate)
            delegate->textChanged(nullptr);
    }
}

void BetterInputNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    selectInput(getWorldSpaceBoundingBox(this).containsPoint(pTouch->getLocation()) && !isNumHoldActive);

    isNumHoldActive = false;
}

void BetterInputNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}

bool BetterInputNode::onDraw(CCTextFieldTTF * sender)
{
    return !useTTFView;
}

void BetterInputNode::setDelegate(TextInputDelegate* delegate)
{
    this->delegate = delegate;
}

TextInputDelegate* BetterInputNode::getDelegate()
{
    return delegate;
}