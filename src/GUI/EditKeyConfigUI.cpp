#include "EditKeyConfigUI.hpp"
#include "BlurLayer.hpp"
#include "BetterButtonSprite.hpp"
#include "../Localisation/LocalisationManager.hpp"

EditKeyConfigUI* EditKeyConfigUI::create(std::function<void(KeyConfigStruct)> onFinish)
{
    auto pRet = new EditKeyConfigUI();

    CCSize size = ccp(260, 180);
    pRet->onFinish = onFinish;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool EditKeyConfigUI::setup()
{
    this->addChild(CCBlurLayer::create(), -3);

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("edit-keybind-ui/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto btn = CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png"), this, menu_selector(EditKeyConfigUI::onClose));
    menu->addChild(btn);

    this->scheduleUpdate();

    

    infoLbl = AdvLabelBMFont::createWithString("", "bigFont.fnt");
    infoLbl->setScale(0.65f);

    auto topRightMenu = CCMenu::create();
    topRightMenu->setContentSize(ccp(0, 300));
    topRightMenu->setAnchorPoint(ccp(0.5f, 1));
    topRightMenu->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setGap(10));
    topRightMenu->setScale(0.75f);
    topRightMenu->addChild(CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "ui/default-button", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(EditKeyConfigUI::onSetDefault)));
    topRightMenu->addChild(CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "ui/undo-button", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(EditKeyConfigUI::onUndoChanged)));
    topRightMenu->updateLayout();

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(topRightMenu, Anchor::TopRight, ccp(-38, -12));
    m_mainLayer->addChildAtPosition(infoLbl, Anchor::Center, ccp(0, 0));
    return true;
}

void EditKeyConfigUI::update(float dt)
{
    if (currentConfig.code != Keycode::KEY_Unknown)
    {
        infoLbl->setOpacity(255);
        infoLbl->setString(currentConfig.configToString().c_str());
    }
    else
    {
        infoLbl->setOpacity(175);
        infoLbl->setString(LocalisationManager::get()->getLocalisedString("edit-keybind-ui/no-keybind").c_str());
    }
}

void EditKeyConfigUI::onSetDefault(CCObject* sender)
{
    this->currentConfig = defaultConfig;
}

void EditKeyConfigUI::onUndoChanged(CCObject* sender)
{
    this->currentConfig = startConfig;
}

void EditKeyConfigUI::onClose(CCObject* sender)
{
    if (onFinish)
        onFinish(currentConfig);

    geode::Popup<>::onClose(sender);
}

void EditKeyConfigUI::setDefaultConfig(KeyConfigStruct config)
{
    this->defaultConfig = config;
}

void EditKeyConfigUI::setStartConfig(KeyConfigStruct config)
{
    this->startConfig = config;
    this->currentConfig = startConfig;
}

void EditKeyConfigUI::keyDown(cocos2d::enumKeyCodes key)
{
    if (key == enumKeyCodes::KEY_Shift || key == enumKeyCodes::KEY_LeftShift || key == enumKeyCodes::KEY_RightShift || key == enumKeyCodes::KEY_None || key == enumKeyCodes::KEY_Unknown || key == enumKeyCodes::KEY_LeftWindowsKey)
        return;

    currentConfig.modifiers.clear();

    if (CCKeyboardDispatcher::get()->getControlKeyPressed())
        currentConfig.modifiers.push_back(KeyModifier::Control);

    if (CCKeyboardDispatcher::get()->getAltKeyPressed())
        currentConfig.modifiers.push_back(KeyModifier::Alt);

    if (CCKeyboardDispatcher::get()->getShiftKeyPressed())
        currentConfig.modifiers.push_back(KeyModifier::Shift);

    if (CCKeyboardDispatcher::get()->getCommandKeyPressed())
        currentConfig.modifiers.push_back(KeyModifier::Command);

    currentConfig.code = key;
}