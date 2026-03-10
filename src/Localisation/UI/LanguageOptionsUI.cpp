#include "LanguageOptionsUI.hpp"
#include "Categories/FavouritesNode.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "BetterButtonSprite.hpp"
#include <Modules/LanguagePreloadFont.hpp>
#include <Modules/TempLoadLanguage.hpp>

LanguageOptionsUI* LanguageOptionsUI::create()
{
    auto pRet = new LanguageOptionsUI();

    CCSize size = ccp(340 + 20, 250);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

LanguageOptionsUI* LanguageOptionsUI::get()
{
    return instance;
}

bool LanguageOptionsUI::setup()
{
    instance = this;

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);
    bg->setPosition(this->m_size / 2);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChild(bg);

    auto title = AdvLabelBMFont::createWithLocalisation("ui/language-options-title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(LanguageOptionsUI::onClose));
    menu->addChild(btn);

    node = CategoryNode::create();
    node->setAnchorPoint(ccp(0.5f, 0.5f));
    node->setContentHeight(170);
    node->updateLayout();
    node->setID("LanguageOptionsUI");

    node->addModule(LanguagePreloadFont::get());
    node->addModule(TempLoadLanguage::get());

    m_mainLayer->addChildAtPosition(node, Anchor::Center, ccp(0, 5));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    return true;
}

LanguageOptionsUI::~LanguageOptionsUI()
{
    instance = nullptr;
}