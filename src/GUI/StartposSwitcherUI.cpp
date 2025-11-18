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

    leftBtn->setPositionX(-65);
    rightBtn->setPositionX(65);

    updateUI();

    this->updateDisplayedOpacity(150);
    this->addChild(label);
    this->addChild(leftBtn);
    this->addChild(rightBtn);
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

void StartposSwitcherUI::updateUI()
{
    if (!PlayLayerUtils::getUtils())
        return;

    label->setString(fmt::format("{} / {}", currentStartpos, PlayLayerUtils::getUtils()->m_fields->startPositions.size()).c_str());

    if (PlayLayerUtils::getUtils()->m_fields->startPositions.size() == 0)
        setScale(0);
}

void StartposSwitcherUI::setStartposVisual(int index)
{
    this->currentStartpos = index;

    updateUI();
}