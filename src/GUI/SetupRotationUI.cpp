#include "SetupRotationUI.hpp"
#include "BlurLayer.hpp"
#include "BetterButtonSprite.hpp"
#include "../Utils/Num.hpp"

SetupRotationUI* SetupRotationUI::create(std::function<void(float)> onFinish)
{
    auto pRet = new SetupRotationUI();

    CCSize size = ccp(300, 220);
    pRet->onFinish = onFinish;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void SetupRotationUI::setDefaultRot(float value)
{
    this->defaultRot = value;
}
void SetupRotationUI::setStartRot(float value)
{
    this->startRot = value;
    this->rot = value;

    updateUI();
}

void SetupRotationUI::onClose(CCObject* sender)
{
    if (onFinish)
        onFinish(rot);

    geode::Popup<>::onClose(sender);
}

void SetupRotationUI::updateUI()
{
    input->setString(floatToStringMin2DP(rot));
    input->setNumHoldValues(true, 1, 5, defaultRot);
    control->setAngle(rot);
}

bool SetupRotationUI::setup()
{
    this->addChild(CCBlurLayer::create(), -3);

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("setup-rotation/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SetupRotationUI::onClose));
    menu->addChild(btn);

    input = BetterInputNode::create(90, "setup-rotation/input-placeholder");
    input->setAlignment(kCCTextAlignmentCenter);
    input->setDelegate(this);
    input->setScale(0.8f);

    control = GJRotationControl::create();
    control->m_delegate = this;

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(input, Anchor::BottomLeft, ccp(50, 60));
    m_mainLayer->addChildAtPosition(control, Anchor::Center, ccp(0, 5));
    return true;
}

void SetupRotationUI::textChanged(CCTextInputNode* node)
{
    if (input->getString().empty())
        return;

    auto str = input->getString();
    float v = utils::numFromString<float>(input->getString()).unwrapOr(rot);

    if (rot != v)
    {
        rot = v;

        updateUI();
    }

    input->setString(str);
}

void SetupRotationUI::angleChanged(float angle)
{
    rot = angle;
    updateUI();
}