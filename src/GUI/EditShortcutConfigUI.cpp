#include "EditShortcutConfigUI.hpp"
#include "BetterButtonSprite.hpp"
#include <Button.hpp>

using namespace qolmod;
using namespace geode::prelude;

EditShortcutConfigUI* EditShortcutConfigUI::create(Module* mod)
{
    auto pRet = new EditShortcutConfigUI();
    CCSize size = ccp(360, 250);
    pRet->mod = mod;
    pRet->config = ShortcutManager::get()->getConfig(mod);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void EditShortcutConfigUI::onClose(CCObject* sender)
{
    PopupBase::onClose(sender);
}

bool EditShortcutConfigUI::setup()
{
    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("setup-shortcut/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();
    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = qolmod::Button::create(spr, this, menu_selector(EditShortcutConfigUI::onClose));
    menu->addChild(btn);

    auto toggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(EditShortcutConfigUI::onTestToggle), 0.75f);
    toggle->setPosition(ccp(0, 110));
    toggle->toggle(config->isEnabled());

    auto label = CCLabelBMFont::create("Enabled", "bigFont.fnt");
    label->setPosition(ccp(25, 110));
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.5f);

    auto label2 = CCLabelBMFont::create("Customizing shortcuts are temporarily broken, sorry :(", "bigFont.fnt");
    label2->setPosition(ccp(0, 70));
    label2->setScale(0.5f);

    this->scheduleUpdate();

    menu->addChildren(toggle, label, label2);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    return true;
}

void EditShortcutConfigUI::onTestToggle(CCObject* sender)
{
    config->setEnabled(!config->isEnabled());
    ShortcutManager::get()->updateConfig(mod);
}

void EditShortcutConfigUI::update(float dt)
{
    
}