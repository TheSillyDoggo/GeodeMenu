#include "StartposSwitcherUI.hpp"
#include "../Hacks/Utils/PlayLayer.hpp"
#include <Button.hpp>

using namespace qolmod;

StartposSwitcherUI* StartposSwitcherUI::create(bool updatingEnabled)
{
    auto pRet = new StartposSwitcherUI();
    pRet->updatingEnabled = updatingEnabled;

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

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    label = CCLabelBMFont::create("6 / 9", "bigFont.fnt");
    label->setScale(0.65f);

    leftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightSpr->setFlipX(true);

    leftSpr->setScale(0.65f);
    rightSpr->setScale(0.65f);

    auto leftBtn = Button::create(leftSpr, this, menu_selector(StartposSwitcherUI::onSwitchBtn));
    auto rightBtn = Button::create(rightSpr, this, menu_selector(StartposSwitcherUI::onSwitchBtn));
    
    leftBtn->setTag(-1);
    rightBtn->setTag(1);

    leftKey = KeycodeNode::create(0);
    rightKey = KeycodeNode::create(0);

    leftKey->setScale(0.75f);
    rightKey->setScale(0.75f);

    leftKey->setPositionX(-56);
    rightKey->setPositionX(56);

    leftPercent = CCLabelBMFont::create("", "bigFont.fnt");
    leftPercent->setScale(0.45f);
    leftPercent->setAnchorPoint(ccp(1, 0.5f));

    rightPercent = CCLabelBMFont::create("", "bigFont.fnt");
    rightPercent->setScale(0.45f);
    rightPercent->setAnchorPoint(ccp(0, 0.5f));

    this->setContentSize(ccp((65 + leftSpr->getScaledContentWidth() / 2) * 2, leftSpr->getScaledContentHeight()));

    updateKeybindsVisualizer(false, 0, 0);
    updateUI();

    this->updateDisplayedOpacity(150);
    this->addChildAtPosition(label, Anchor::Center);
    this->addChildAtPosition(leftBtn, Anchor::Left, ccp(leftSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(rightBtn, Anchor::Right, ccp(-rightSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(leftKey, Anchor::Left, ccp(-9 + leftSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(rightKey, Anchor::Right, ccp(9 - rightSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(leftPercent, Anchor::Left, ccp(-25 + leftSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(rightPercent, Anchor::Right, ccp(25 - rightSpr->getScaledContentWidth() / 2, 0));
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
    leftPercent->setOpacity(parentOpacity);
    rightPercent->setOpacity(parentOpacity);
}

void StartposSwitcherUI::updateKeybindsVisualizer(bool visible, int leftCode, int rightCode)
{
    visible = false;

    leftKey->setKeycode(leftCode);
    rightKey->setKeycode(rightCode);

    leftKey->setVisible(visible);
    rightKey->setVisible(visible);
}

float StartposSwitcherUI::getPercentForObject(GameObject* go)
{
    if (!go || !PlayLayer::get())
        return 0;

    auto len = PlayLayer::get()->m_levelLength;

    if (len == 0)
        len = 1;

    return (go->getPositionX() / len) * 100;
}

void StartposSwitcherUI::updateUI()
{
    if (!updatingEnabled)
    {
        leftPercent->setString("");
        rightPercent->setString("");
        label->setString("6 / 9");
        return;
    }

    if (!PlayLayerUtils::getUtils())
        return;

    label->setString(fmt::format("{} / {}", currentStartpos, PlayLayerUtils::getUtils()->m_fields->startPositions.size()).c_str());

    if (usePercentage)
    {
        auto index = currentStartpos;
        auto startPoses = PlayLayerUtils::getUtils()->m_fields->startPositions;
        float percent = getPercentForObject(index == 0 ? nullptr : startPoses[index - 1]);

        label->setString(fmt::format("{}%", utils::numToString<float>(percent, 2)).c_str());

        index = currentStartpos - 1;
        if (index == -1)
            index = startPoses.size();

        percent = getPercentForObject(index == 0 ? nullptr : startPoses[index - 1]);
        
        leftPercent->setString(fmt::format("{}%", utils::numToString<float>(percent, 2)).c_str());

        index = currentStartpos + 1;
        if (index == startPoses.size() + 1)
            index = 0;

        percent = getPercentForObject(index == 0 ? nullptr : startPoses[index - 1]);
        
        rightPercent->setString(fmt::format("{}%", utils::numToString<float>(percent, 2)).c_str());
    }
    else
    {
        leftPercent->setString("");
        rightPercent->setString("");
    }

    if (PlayLayerUtils::getUtils()->m_fields->startPositions.size() == 0)
        setScale(0);
}

void StartposSwitcherUI::setStartposVisual(int index)
{
    this->currentStartpos = index;

    updateUI();
}