#include "SearchFiltersUI.hpp"
#include "Categories/SearchNode.hpp"

SearchFiltersUI* SearchFiltersUI::create()
{
    auto pRet = new SearchFiltersUI();

    CCSize size = ccp(300, 200);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool SearchFiltersUI::setup()
{
    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = CCLabelBMFont::create("Filter search results", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto btn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SearchFiltersUI::onClose));
    menu->addChild(btn);

    auto node = CategoryNode::create();
    node->setAnchorPoint(ccp(0.5f, 0.5f));
    node->setContentSize(ccp(275, 110));
    node->addModule(SearchShowOptions::get());

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(node, Anchor::Center, ccp(0, 5));
    return true;
}