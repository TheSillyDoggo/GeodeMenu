#include "SetupLabelConfigUI.hpp"
#include "../LabelManager.hpp"
#include "../../GUI/CategoryTabSprite.hpp"

SetupLabelConfigUI* SetupLabelConfigUI::create(std::function<void(LabelConfig)> onFinish)
{
    auto pRet = new SetupLabelConfigUI();

    CCSize size = ccp(360, 250);
    pRet->onFinish = onFinish;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool SetupLabelConfigUI::setup()
{
    this->scheduleUpdate();

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto menu = CCMenu::create();

    auto btn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupLabelConfigUI::onClose));
    menu->addChild(btn);

    anchorMenu = CCMenu::create();
    anchorMenu->setAnchorPoint(ccp(0, 0));
    anchorMenu->setScale(1.3f);
    anchorMenu->setContentSize(ccp(0, 0));

    pagesMenu = CCMenu::create();
    pagesMenu->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setAxisReverse(false)->setAxisAlignment(AxisAlignment::Center)->setGap(7.5f));
    pagesMenu->ignoreAnchorPointForPosition(false);
    pagesMenu->setContentSize(ccp(770, 20));

    createPages();
    createAnchorNodes();
    updateUI();

    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(anchorMenu, Anchor::TopRight, ccp(-100, -100));
    m_mainLayer->addChildAtPosition(pagesMenu, Anchor::Top, ccp(0, -22));
    return true;
}

void SetupLabelConfigUI::onClose(CCObject* sender)
{
    if (onFinish)
        onFinish(currentConfig);

    geode::Popup<>::onClose(sender);
}

void SetupLabelConfigUI::updateUI()
{
    for (auto anchor : anchorBtns)
    {
        anchor.second->setEnabled(anchor.first != currentConfig.anchor);
        anchor.second->toggle(anchor.first == currentConfig.anchor);
    }

    for (auto page : pages)
    {
        page->setVisible(page->getTag() == selectedPage);
    }

    for (auto toggler : pageBtns)
    {
        toggler->setEnabled(toggler->getTag() != selectedPage);
        toggler->toggle(toggler->getTag() == selectedPage);
    }
}

void SetupLabelConfigUI::createPages()
{
    std::vector<std::string> pageNames = { "General", "Format", "Events" };

    for (size_t i = 0; i < pageNames.size(); i++)
    {
        auto pg = CCNode::create();
        pg->setContentSize(m_size);
        pg->setTag(i);
        pg->setZOrder(2);
        pages.push_back(pg);

        this->addChild(pg);

        // fmt::format("{}{}.png", ""_spr, utils::string::toLower(pageNames[i]))
        auto off = CategoryTabSprite::create(CategoryTabType::Text, pageNames[i], "");
        off->setContentSize(ccp(80, 20));
        off->updateSelection(CategorySelectionType::Deselected);

        auto held = CategoryTabSprite::create(CategoryTabType::Text, pageNames[i], "");
        held->setContentSize(ccp(80, 20));
        held->updateSelection(CategorySelectionType::Hovered);

        auto on = CategoryTabSprite::create(CategoryTabType::Text, pageNames[i], "");
        on->setContentSize(ccp(80, 20));
        on->updateSelection(CategorySelectionType::Selected);

        auto toggler = CCMenuItemToggler::create(off, on, this, menu_selector(SetupLabelConfigUI::onChangePage));
        toggler->setTag(i);
        toggler->m_offButton->m_scaleMultiplier = 1;
        toggler->m_offButton->setSelectedImage(held);
        toggler->m_onButton->m_scaleMultiplier = 1;

        off->setAnchorPoint(ccp(0, 0));
        held->setPosition(held->getContentSize() / 2);
        on->setAnchorPoint(ccp(0, 0));

        pagesMenu->addChild(toggler);
        pageBtns.push_back(toggler);
    }

    pagesMenu->updateLayout();
}

void SetupLabelConfigUI::createAnchorNodes()
{
    for (size_t i = 0; i < 9; i++)
    {
        auto anchor = (LabelAnchor)i;
        std::string spr = "";
        float rot = 0;

        switch (anchor)
        {
            case LabelAnchor::TopLeft:
                spr = "anchor_corner.png"_spr;
                break;
            
            case LabelAnchor::TopCenter:
                spr = "anchor_side.png"_spr;
                rot = 90;
                break;

            case LabelAnchor::TopRight:
                spr = "anchor_corner.png"_spr;
                rot = 90;
                break;

            case LabelAnchor::CenterRight:
                spr = "anchor_side.png"_spr;
                rot = 180;
                break;

            case LabelAnchor::BottomRight:
                spr = "anchor_corner.png"_spr;
                rot = 180;
                break;

            case LabelAnchor::BottomCenter:
                spr = "anchor_side.png"_spr;
                rot = 270;
                break;

            case LabelAnchor::BottomLeft:
                spr = "anchor_corner.png"_spr;
                rot = 270;
                break;

            case LabelAnchor::CenterLeft:
                spr = "anchor_side.png"_spr;
                break;

            case LabelAnchor::Center:
                spr = "anchor_center.png"_spr;
                break;
        }

        auto sprNormal = CCSprite::create(spr.c_str());
        sprNormal->setRotation(rot);
        sprNormal->setColor(ccc3(150, 150, 150));

        auto sprHeld = CCSprite::create(spr.c_str());
        sprHeld->setRotation(rot);
        sprHeld->setColor(ccc3(200, 200, 200));

        auto sprSel = CCSprite::create(spr.c_str());
        sprSel->setRotation(rot);

        auto toggler = CCMenuItemToggler::create(sprNormal, sprSel, this, menu_selector(SetupLabelConfigUI::onSetAnchor));
        toggler->m_offButton->setSelectedImage(sprHeld);
        toggler->m_offButton->m_scaleMultiplier = 1;
        toggler->m_onButton->m_scaleMultiplier = 1;

        toggler->setTag(i);

        sprHeld->setPosition(sprHeld->getContentSize() / 2);
        sprHeld->setAnchorPoint(ccp(0.5f, 0.5f));

        toggler->setPosition(LabelManager::get()->anchorToPoint(anchor) * 18 * 2);
        anchorMenu->addChild(toggler);

        anchorBtns.emplace(anchor, toggler);
    }
}

void SetupLabelConfigUI::onSetAnchor(CCObject* sender)
{
    currentConfig.anchor = (LabelAnchor)sender->getTag();

    updateUI();
}

void SetupLabelConfigUI::onChangePage(CCObject* sender)
{
    this->selectedPage = sender->getTag();

    updateUI();
}

void SetupLabelConfigUI::setStartConfig(LabelConfig conf)
{
    this->startConfig = conf;
    this->currentConfig = conf;

    updateUI();
}