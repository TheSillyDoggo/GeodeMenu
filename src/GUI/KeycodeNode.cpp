#include "KeycodeNode.hpp"

KeycodeNode* KeycodeNode::create(int keycode)
{
    auto pRet = new KeycodeNode();

    if (pRet && pRet->init(keycode))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void KeycodeNode::setKeycode(int keycode)
{
    this->keycode = keycode;
    auto str = CCKeyboardDispatcher::get()->keyToString(static_cast<enumKeyCodes>(keycode));

    label->setString(str ? str : "None");
    label->limitLabelWidth(getContentWidth() - 2, 0.5f, 0);
}

int KeycodeNode::getKeycode()
{
    return this->keycode;
}

bool KeycodeNode::init(int keycode)
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentSize(ccp(20, 20));

    bg = CCScale9Sprite::create("geode.loader/black-square.png");
    bg->setScale(0.5f);
    bg->setContentSize(getContentSize() * 2);

    label = CCLabelBMFont::create("", "goldFont.fnt");
    label->setScale(0.5f);

    setKeycode(keycode);
    
    this->addChildAtPosition(bg, Anchor::Center);
    this->addChildAtPosition(label, Anchor::Center);

    return true;
}

void KeycodeNode::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    if (bg)
        bg->setContentSize(contentSize * 2);
    
    if (label)
        label->limitLabelWidth(getContentWidth() - 2, 0.5f, 0);

    updateLayout();
}