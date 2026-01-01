#include "BetterInputNode.hpp"
#include "../Utils/Num.hpp"
#include "../Utils/RealtimeAction.hpp"

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
    cursorCarot->runAction(RealtimeAction::create(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.1f, 175), CCDelayTime::create(0.4f), CCFadeTo::create(0.1f, 125), CCDelayTime::create(0.4f), nullptr))));

    labelContainer = CCNode::create();
    labelContainer->setAnchorPoint(ccp(0.5f, 0.5f));

    placeholderLbl = AdvLabelBMFont::createWithString(placeholder, font);
    placeholderLbl->setColor(ccc3(150, 150, 150));
    placeholderLbl->setAnchorPoint(ccp(0, 0));
    labelContainer->addChild(placeholderLbl);

    textLbl = AdvLabelBMFont::createWithStruct({}, font);
    textLbl->setShowTags(true);
    textLbl->setAnchorPoint(ccp(0, 0));

    textLblUser = CCLabelTTF::create("", "Arial.ttf", 32);
    textLblUser->setAnchorPoint(ccp(0, 0));

    labelContainer->addChild(textLbl);
    labelContainer->addChild(textLblUser);
    labelContainer->addChild(cursorCarot);

    this->addChild(bg);
    this->addChild(ttfInput);
    this->addChild(labelContainer);
    return true;
}

void BetterInputNode::selectInput(bool selected)
{
    isSelected = selected;

    if (selected)
    {
        ttfInput->attachWithIME();
        bg->getBG()->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, 125)));
    }
    else
    {
        ttfInput->detachWithIME();
        bg->getBG()->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, 100)));
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
    cursorCarot->setPositionY(labelContainer->getContentHeight() / 2);
    cursorCarot->setScale(1.0f / labelContainer->getScale());

    updateCursorPos(false, CCPointZero);

    CCMenu::visit();
}

void BetterInputNode::setString(std::string str)
{
    str = filterString(str);

    this->text = str;

    ttfInput->setString(str.c_str());
    textLbl->setString(str.c_str());

    useTTFView = textLbl->doesLabelContainNonDisplayableCharacter();

    if (useTTFView)
    {
        textLblUser->setString(str.c_str());
    }

    #ifdef GEODE_IS_DESKTOP
    ttfInput->m_uCursorPos = str.size() + 1;
    #endif

    updateCursorPos(false, CCPointZero);
}

std::string BetterInputNode::filterString(std::string ss)
{
    std::string s;
    std::string s2;

    if (maxChars != -1)
        s = ss.substr(0, maxChars);
    else
        s = ss;
    
    if (!charFilter.empty())
    {
        for (auto ch : s)
        {
            for (auto ch2 : charFilter)
            {
                if (ch2 == ch)
                {
                    s2 += ch;
                    break;
                }
            }
            
        }
    }
    else
    {
        s2 += s;
    }

    return s2;
}

void BetterInputNode::setMaxChars(int max)
{
    this->maxChars = max;
}

void BetterInputNode::setCharFilter(std::string str)
{
    this->charFilter = str;
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
    bg->getBG()->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, 125)));
    isSelected = true;

    return false;
}

bool BetterInputNode::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    bg->getBG()->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, 100)));
    isSelected = false;
    
    return false;
}

bool BetterInputNode::onTextFieldInsertText(CCTextFieldTTF * sender, const char * _text, int nLen, cocos2d::enumKeyCodes code)
{
    if (!_text)
        return true;

    if (text == "\n" && nLen == 1)
        return true; 

    if (CCKeyboardDispatcher::get()->getControlKeyPressed())
    {
        if (code == enumKeyCodes::KEY_Left)
        {
            int by = -1;

            char lastChar = 'A';

            for (int i = getRealCursorPos() - 1; i > 0; i--)
            {
                if (text[i] == ' ' && lastChar != ' ')
                {
                    break;
                }

                by--;
                lastChar = text[i];
            }

            moveCursor(by);
        }

        if (code == enumKeyCodes::KEY_Right)
        {
            int by = 1;

            char lastChar = 'A';

            for (int i = getRealCursorPos(); i < text.size(); i++)
            {
                if (text[i] != ' ' && lastChar == ' ')
                {
                    by--;
                    break;
                }

                by++;
                lastChar = text[i];
            }

            moveCursor(by);
        }
    }
    else
    {
        if (code == enumKeyCodes::KEY_Left)
        {
            moveCursor(-1);
        }

        if (code == enumKeyCodes::KEY_Right)
        {
            moveCursor(1);
        }
    }

    if (code == enumKeyCodes::KEY_Unknown)
    {
        #if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)
        this->text = "";
        std::string str = std::string(_text);
        #else
        std::string str = this->text;
        str.insert(getRealCursorPos(), _text);
        #endif
        
        setString(str);

        moveCursor(nLen);

        if (delegate)
            delegate->textChanged(nullptr);
    }
    
    return false;
}

bool BetterInputNode::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    if (getRealCursorPos() == 0)
        return false;
    
    auto cPos = cursorPos;

    moveCursor(-nLen);
    
    this->text.erase(getRealCursorPos(), nLen);
    setString(this->text);

    if (cPos == -1)
        cursorPos = -1;

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
    if (auto n = getTopLevelNonSceneNode(this))
    {
        if (CCScene::get()->getChildByIndex(-1) != n)
            return false;
    }

    if (getWorldSpaceBoundingBox(this).containsPoint(pTouch->getLocation()) && nodeIsVisible(this))
    {
        bg->getBG()->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, 125)));

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

        std::string ss;

        if ((int)v == v)
            ss = utils::numToString<int>(v, 0);
        else
            ss = utils::numToString<float>(v, 2);

        setString(ss);

        if (delegate)
            delegate->textChanged(nullptr);
    }
}

void BetterInputNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    updateCursorPos(true, pTouch->getLocation());
    
    selectInput(getWorldSpaceBoundingBox(this).containsPoint(pTouch->getLocation()) && !isNumHoldActive);

    isNumHoldActive = false;
}

void BetterInputNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}

bool BetterInputNode::onDraw(CCTextFieldTTF * sender)
{
    return false;
}

void BetterInputNode::setDelegate(TextInputDelegate* delegate)
{
    this->delegate = delegate;
}

TextInputDelegate* BetterInputNode::getDelegate()
{
    return delegate;
}

int BetterInputNode::getRealCursorPos()
{
    if (cursorPos == -1)
        return text.size();

    return cursorPos;
}

void BetterInputNode::moveCursor(int by)
{
    #if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)
    updateCursorPos(false, CCPointZero);
    cursorPos = -1;
    return;
    #endif

    if (cursorPos == -1)
    {
        if (by > 0)
            return;
        else
            cursorPos = text.size();
    }

    cursorPos += by;

    if (cursorPos < 0)
        cursorPos = 0;

    if (cursorPos >= text.size())
        cursorPos = -1;

    updateCursorPos(false, CCPointZero);
}

void BetterInputNode::updateCursorPos(bool isTouchUpdate, CCPoint touchPos)
{
    auto nodes = textLbl->getCharacterNodes();

    if (isTouchUpdate)
    {
        float maxX = getWorldSpaceBoundingBox(textLbl).getMinX();
        float minX = getWorldSpaceBoundingBox(textLbl).getMinX();

        int i = 0;
        for (auto node : nodes)
        {
            auto bb = getWorldSpaceBoundingBox(node);

            if (touchPos.x > bb.getMinX() - bb.size.width / 2)
                cursorPos = i;

            maxX = std::max<float>(bb.getMidX(), maxX);
            i++;
        }

        if (touchPos.x > maxX)
        {
            cursorPos = -1;
        }

        if (touchPos.x < minX)
        {
            cursorPos = 0;
        }
    }

    auto cPos = getRealCursorPos();

    if (nodes.size() > cPos)
    {
        if (auto n = nodes[cPos])
        {
            cursorCarot->setPositionX((n->getPositionX() - n->getScaledContentWidth() / 2) + n->getParent()->getPositionX());
        }
        else
        {
            cursorCarot->setPositionX(labelContainer->getContentWidth());
        }
    }
    else
    {
        cursorCarot->setPositionX(labelContainer->getContentWidth());
    }
}