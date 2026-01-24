#include "ExtraThemeSettingsUI.hpp"
#include "Modules/GradientBGColours.hpp"

#include "BetterButtonSprite.hpp"

ExtraThemeSettingsUI* ExtraThemeSettingsUI::create()
{
    auto pRet = new ExtraThemeSettingsUI();

    CCSize size = ccp(300, 220);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool ExtraThemeSettingsUI::setup()
{
    

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("extra-theme-settings/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ExtraThemeSettingsUI::onClose));
    menu->addChild(btn);

    auto bg = CCScale9Sprite::create("square02b_small.png");
    bg->setContentSize(ccp(165, 120) / 0.5f);
    bg->setScale(0.5f);
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);

    auto mod1 = GradientBGStart::get()->getNode();
    auto mod2 = GradientBGEnd::get()->getNode();
    auto mod3 = GradientBGDirection::get()->getNode();

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(bg, Anchor::Center, ccp(0, 5));
    m_mainLayer->addChildAtPosition(mod1, Anchor::Center, ccp(6, 5 + 40));
    m_mainLayer->addChildAtPosition(mod2, Anchor::Center, ccp(6, 5));
    m_mainLayer->addChildAtPosition(mod3, Anchor::Center, ccp(6, 5 - 40));
    return true;
}