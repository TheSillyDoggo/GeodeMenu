#include "EasyBG.hpp"

bool EasyBG::init()
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(0.5f, 0.5f));

    bg = CCScale9Sprite::create("square02b_small.png");
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);
    bg->setScale(0.5f);

    this->addChild(bg);
    return true;
}

void EasyBG::visit()
{
    if (targetting)
        this->setContentSize(targetting->getScaledContentSize() + targettingOffset);

    bg->setContentSize(getContentSize() / 0.5f);
    bg->setPosition(getContentSize() / 2);

    CCNode::visit();
}

void EasyBG::setTargettingNode(CCNode* target)
{
    this->targetting = target;
}

void EasyBG::setTargettingOffset(CCSize offset)
{
    this->targettingOffset = offset;
}

CCScale9Sprite* EasyBG::getBG()
{
    return bg;
}