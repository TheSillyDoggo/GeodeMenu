#include "EditKeyConfigUI.hpp"
#include "BetterButtonSprite.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include <Button.hpp>
#include <BetterAlertLayer.hpp>

using namespace qolmod;

EditKeyConfigUI* EditKeyConfigUI::create(std::function<void(KeyConfigStruct)> onFinish)
{
    auto pRet = new EditKeyConfigUI();

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

bool EditKeyConfigUI::setup()
{
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("edit-keybind-ui/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto btn = Button::create(BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png"), this, menu_selector(EditKeyConfigUI::onClose));
    menu->addChild(btn);

    this->scheduleUpdate();


    infoLbl = AdvLabelBMFont::createWithString("", "bigFont.fnt");
    infoLbl->setScale(0.65f);

    auto topRightMenu = CCMenu::create();
    topRightMenu->setContentSize(ccp(0, 300));
    topRightMenu->setAnchorPoint(ccp(0.5f, 1));
    topRightMenu->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setGap(10));
    topRightMenu->setScale(0.75f);
    topRightMenu->addChild(Button::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "ui/default-button", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(EditKeyConfigUI::onSetDefault)));
    topRightMenu->addChild(Button::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "ui/undo-button", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(EditKeyConfigUI::onUndoChanged)));
    topRightMenu->updateLayout();

    auto modeMenu = CCMenu::create();

    createModeButton(KeybindType::Toggle, 2, modeMenu);
    createModeButton(KeybindType::Hold, 1, modeMenu);
    createModeButton(KeybindType::HoldInverted, 0, modeMenu);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(topRightMenu, Anchor::TopRight, ccp(-38, -12));
    m_mainLayer->addChildAtPosition(infoLbl, Anchor::Center, ccp(0, 0));
    m_mainLayer->addChildAtPosition(modeMenu, Anchor::BottomLeft, ccp(20, 20));
    return true;
}

void EditKeyConfigUI::update(float dt)
{
    if (currentConfig.code != Keycode::KEY_Unknown)
    {
        infoLbl->setOpacity(255);
        infoLbl->setString(fmt::format("<bm>{}", currentConfig.configToString()).c_str());
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
    updateMode(nullptr);
}

void EditKeyConfigUI::onUndoChanged(CCObject* sender)
{
    this->currentConfig = startConfig;
    updateMode(nullptr);
}

void EditKeyConfigUI::onSetKeyMode(CCObject* sender)
{
    currentConfig.type = (KeybindType)sender->getTag();
    
    updateMode(nullptr);
}

void EditKeyConfigUI::createModeButton(KeybindType type, int y, CCMenu* menu)
{
    auto toggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(EditKeyConfigUI::onSetKeyMode), 0.65f);
    toggle->setTag((int)type);
    toggle->setPosition(ccp(0, 27 * y));

    std::string key = "";

    switch (type)
    {
        case KeybindType::Toggle:
            key = "edit-keybind-ui/toggle-mode-title";
            break;

        case KeybindType::Hold:
            key = "edit-keybind-ui/hold-mode-title";
            break;

        case KeybindType::HoldInverted:
            key = "edit-keybind-ui/hold-inverted-mode-title";
            break;
    }

    auto label = AdvLabelBMFont::createWithLocalisation(key, "goldFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccp(16, toggle->getPositionY()));
    label->limitLabelWidth(60, 0.55f, 0);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.4f + 0.85f);

    auto infoBtn = Button::create(infoSpr, this, menu_selector(EditKeyConfigUI::onModeInfo));
    infoBtn->setTag((int)type);
    infoSpr->setScale(infoSpr->getScale() - 0.85f);
    infoBtn->setPosition(label->getPosition() + ccp(label->getScaledContentWidth(), 0) + ccp(4.5f, 5));

    menu->addChild(toggle);
    menu->addChild(label, 2);
    menu->addChild(infoBtn, 3);
    modeBtns.emplace(type, std::make_pair(toggle, label));
}

void EditKeyConfigUI::updateMode(CCMenuItemToggler* except)
{
    for (auto& modeData : modeBtns)
    {
        modeData.second.first->setEnabled(modeData.first != currentConfig.type);
        modeData.second.second->setColor(modeData.first == currentConfig.type ? ccWHITE : ccc3(150, 150, 150));

        if (modeData.second.first != except)
            modeData.second.first->toggle(modeData.first == currentConfig.type);
    }
}

void EditKeyConfigUI::onClose(CCObject* sender)
{
    if (onFinish)
        onFinish(currentConfig);

    PopupBase::onClose(sender);
}

void EditKeyConfigUI::onModeInfo(CCObject* sender)
{
    std::string key = "";
    std::string key2 = "";

    switch ((KeybindType)sender->getTag())
    {
        case KeybindType::Toggle:
            key = "edit-keybind-ui/toggle-mode-title";
            key2 = "edit-keybind-ui/toggle-mode-info";
            break;

        case KeybindType::Hold:
            key = "edit-keybind-ui/hold-mode-title";
            key2 = "edit-keybind-ui/hold-mode-info";
            break;

        case KeybindType::HoldInverted:
            key = "edit-keybind-ui/hold-inverted-mode-title";
            key2 = "edit-keybind-ui/hold-inverted-mode-info";
            break;
    }

    BetterAlertLayer::createWithLocalisation(key.c_str(), key2.c_str(), "ui/ok-button")->show();
}

void EditKeyConfigUI::setDefaultConfig(KeyConfigStruct config)
{
    this->defaultConfig = config;
}

void EditKeyConfigUI::setStartConfig(KeyConfigStruct config)
{
    this->startConfig = config;
    this->currentConfig = startConfig;

    updateMode(nullptr);
}

void EditKeyConfigUI::keyDown(cocos2d::enumKeyCodes key, double timestamp)
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