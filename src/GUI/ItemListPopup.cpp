#include "../Localisation/LocalisationManager.hpp"
#include "BetterButtonSprite.hpp"
#include "EasyBG.hpp"
#include <Button.hpp>
#include "ItemListPopup.hpp"

using namespace qolmod;

ItemListPopup* ItemListPopup::create(char const* title, const char* btn)
{
    auto pRet = new ItemListPopup();
    pRet->m_title = title;
    pRet->m_button = btn;
    if (pRet && pRet->initAnchored(330, 140))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ItemListPopup::addItem(const std::string& text)
{
    addItem("", text);
}

void ItemListPopup::addItem(const std::string& sprite, const std::string& text, bool frame)
{
    constexpr float CELL_HEIGHT = 35;
    constexpr float CELL_INSET = 4.5f;

    auto container = CCNode::create();
    container->setContentSize(ccp(m_scrollLayer->getContentWidth(), CELL_HEIGHT));

    auto bg = EasyBG::create();
    bg->setTargettingNode(container);
    container->addChildAtPosition(bg, Anchor::Center);

    float xInset = CELL_INSET;
    if (!sprite.empty())
    {
        auto spr = frame ? CCSprite::createWithSpriteFrameName(sprite.c_str()) : CCSprite::create(sprite.c_str());
        spr->setScale((CELL_HEIGHT - (CELL_INSET * 2)) / spr->getContentHeight());
        spr->setPosition(ccp(xInset + spr->getScaledContentWidth() / 2, CELL_HEIGHT / 2));
        container->addChild(spr);

        xInset += spr->getScaledContentWidth() + CELL_INSET;
    }

    auto lbl = AdvLabelBMFont::createWithString(text, "bigFont.fnt");
    lbl->setAnchorPoint(ccp(0, 0.5f));
    lbl->setScale(0.4f);
    lbl->setMaxWidth((container->getContentWidth() - xInset - CELL_INSET) / 0.4f);

    if (sprite.empty())
        container->setContentHeight(lbl->getScaledContentHeight() + CELL_INSET * 2);

    container->addChildAtPosition(lbl, Anchor::Left, ccp(xInset, 0));

    container->updateLayout();
    m_scrollLayer->m_contentLayer->addChild(container);

    updateSize();
}

void ItemListPopup::onClose(CCObject* sender)
{
    PopupBase::onClose(sender);
}

void ItemListPopup::updateSize()
{
    m_scrollLayer->m_contentLayer->updateLayout();

    m_mainLayer->setContentHeight(std::clamp<float>(m_scrollLayer->m_contentLayer->getContentHeight() + 100, 140, 220));
    m_mainLayer->updateLayout();

    m_scrollLayer->setContentHeight(m_mainLayer->getContentHeight() - 100);
    m_bg->setContentSize(m_mainLayer->getContentSize());

    m_scrollLayer->moveToTop();

    m_scrollBar->setVisible(m_scrollLayer->m_contentLayer->getContentHeight() > m_scrollLayer->getContentHeight());
    m_scrollLayer->setTouchEnabled(m_scrollBar->isVisible());
}

bool ItemListPopup::setup()
{
    m_bgSprite->setVisible(false);
    m_buttonMenu->setVisible(false);

    m_bg = CCScale9Sprite::create("square01_001.png");
    m_bg->setContentSize(ccp(300, 140));

    m_titleLabel = AdvLabelBMFont::createWithString(m_title, "goldFont.fnt");
    m_titleLabel->setAnchorPoint(ccp(0.5f, 1));
    m_titleLabel->limitLabelWidth(m_bg->getContentWidth() - 60, 0.9f, 0);

    m_scrollLayer = qolmod::ScrollLayer::create(ccp(m_bg->getContentWidth() - 35, m_bg->getContentHeight() - 60));
    m_scrollLayer->m_contentLayer->setLayout(m_scrollLayer->createDefaultListLayout());

    m_scrollBar = Scrollbar::create(m_scrollLayer, Scrollbar::Style::GDScrollbar);
    m_scrollBar->setAnchorPoint(ccp(1, 0));
    m_scrollBar->setZOrder(69);
    m_scrollLayer->addChildAtPosition(m_scrollBar, Anchor::BottomRight);

    auto menu = CCMenu::create();
    menu->setZOrder(8);

    auto spr = BetterButtonSprite::create(ccp(54.25f, 30), m_button, "goldFont.fnt", "GJ_button_01.png");
    spr->fixSize();
    auto btn = qolmod::Button::create(spr, this, menu_selector(ItemListPopup::onClose));
    btn->setTag(1);
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(m_bg, Anchor::Center);
    m_mainLayer->addChildAtPosition(m_scrollLayer, Anchor::Center, ccp(0, 2.5f));
    m_mainLayer->addChildAtPosition(m_titleLabel, Anchor::Top, ccp(0, -15));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 30));
    return false;
}