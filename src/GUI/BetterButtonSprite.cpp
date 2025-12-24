#include "BetterButtonSprite.hpp"

BetterButtonSprite* BetterButtonSprite::create(CCSize size, std::string str, std::string fnt, std::string bg)
{
    auto pRet = create();

    pRet->setContentSize(size);
    pRet->setString(str);
    pRet->setFont(fnt);
    pRet->setBGSprite(bg);

    return pRet;
}

bool BetterButtonSprite::init()
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(0.5f, 0.5f));

    label = AdvLabelBMFont::createWithString("", "bigFont.fnt");

    setBGSprite("GJ_button_01.png");

    this->addChild(label);
    return true;
}

void BetterButtonSprite::visit()
{
    background->setContentSize(getContentSize());
    label->setPosition(getContentSize() / 2 + ccp(0, 2));
    label->limitLabelWidth(getContentWidth() - 12, 1.0f, 0);

    CCNode::visit();
}

void BetterButtonSprite::setBGSprite(std::string file)
{
    if (background)
    {
        background->removeFromParent();
    }

    background = CCScale9Sprite::create(file.c_str());
    background->setAnchorPoint(ccp(0, 0));
    background->setPosition(ccp(0, 0));

    this->addChild(background, -1);
}

void BetterButtonSprite::setFont(std::string font)
{
    label->setFntFile(font.c_str());
}

void BetterButtonSprite::setString(std::string str)
{
    label->setString(str.c_str());
}