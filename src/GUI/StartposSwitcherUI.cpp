#include "StartposSwitcherUI.hpp"
#include "../Hacks/Utils/PlayLayer.hpp"

StartposSwitcherUI* StartposSwitcherUI::create()
{
    auto pRet = new StartposSwitcherUI();

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool StartposSwitcherUI::init()
{
    if (!CCMenu::init())
        return false;

    label = CCLabelBMFont::create("6 / 9", "bigFont.fnt");
    label->setScale(0.65f);

    leftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightSpr->setFlipX(true);

    leftSpr->setScale(0.65f);
    rightSpr->setScale(0.65f);

    auto leftBtn = CCMenuItemSpriteExtra::create(leftSpr, this, menu_selector(StartposSwitcherUI::onSwitchBtn));
    auto rightBtn = CCMenuItemSpriteExtra::create(rightSpr, this, menu_selector(StartposSwitcherUI::onSwitchBtn));
    
    leftBtn->setTag(-1);
    rightBtn->setTag(1);

    leftKey = KeycodeNode::create(0);
    rightKey = KeycodeNode::create(0);

    leftKey->setScale(0.75f);
    rightKey->setScale(0.75f);

    leftBtn->setPositionX(-65);
    rightBtn->setPositionX(65);

    leftKey->setPositionX(-56);
    rightKey->setPositionX(56);

    updateUI();

    this->updateDisplayedOpacity(150);
    this->addChild(label);
    this->addChild(leftBtn);
    this->addChild(rightBtn);
    this->addChild(leftKey);
    this->addChild(rightKey);
    return true;
}

void StartposSwitcherUI::onSwitchBtn(CCObject* sender)
{
    if (onSwitchFunc)
        onSwitchFunc(sender->getTag());
}

void StartposSwitcherUI::updateDisplayedOpacity(GLubyte parentOpacity)
{
    label->setOpacity(parentOpacity);
    leftSpr->setOpacity(parentOpacity);
    rightSpr->setOpacity(parentOpacity);
}

void StartposSwitcherUI::updateKeybindsVisualizer(bool visible, int leftCode, int rightCode)
{
    leftKey->setKeycode(leftCode);
    rightKey->setKeycode(rightCode);

    leftKey->setVisible(visible);
    rightKey->setVisible(visible);
}

void StartposSwitcherUI::updateUI()
{
    if (!PlayLayerUtils::getUtils())
        return;

    label->setString(fmt::format("{} / {}", currentStartpos, PlayLayerUtils::getUtils()->m_fields->startPositions.size()).c_str());

    if (true)
    {
        // auto spos = index == 0 ? nullptr : PlayLayerUtils::getUtils()->m_fields->startPositions[m_fields->startPosIndex - 1];
    }

    if (PlayLayerUtils::getUtils()->m_fields->startPositions.size() == 0)
        setScale(0);
}

void StartposSwitcherUI::setStartposVisual(int index)
{
    this->currentStartpos = index;

    updateUI();
}