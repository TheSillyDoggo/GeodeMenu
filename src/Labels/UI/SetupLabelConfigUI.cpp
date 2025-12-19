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
    
    createPage1();
    createPage2();
    createPage3();

    updateUI();

    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(pagesMenu, Anchor::Top, ccp(0, -22));
    return true;
}

void SetupLabelConfigUI::onClose(CCObject* sender)
{
    currentConfig.cheatIndicator = cheatIndicatorToggler->isToggled();
    currentConfig.noclipOnly = noclipOnlyToggler->isToggled();

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

    formatInp->setString(currentConfig.formatString);

    nameInp->setString(currentConfig.displayName);
    scaleInp->setString(utils::numToString<float>(currentConfig.scale, 2));
    opacityInp->setString(utils::numToString<float>(currentConfig.opacity, 2));

    offsetXInp->setString(utils::numToString<float>(currentConfig.offset.x, 2));
    offsetYInp->setString(utils::numToString<float>(currentConfig.offset.y, 2));

    cheatIndicatorToggler->toggle(currentConfig.cheatIndicator);
    noclipOnlyToggler->toggle(currentConfig.noclipOnly);
}

void SetupLabelConfigUI::createPage1()
{
    createAnchorNodes();

    nameInp = TextInput::create(160, "Display Name", "bigFont.fnt");
    nameInp->setAnchorPoint(ccp(0, 0.5f));
    nameInp->setScale(0.7f);
    nameInp->setCommonFilter(CommonFilter::Any);
    nameInp->setCallback([this](const std::string& str)
    {
        currentConfig.displayName = str;
    });

    scaleInp = TextInput::create(160, "Scale", "bigFont.fnt");
    scaleInp->setAnchorPoint(ccp(0, 0.5f));
    scaleInp->setScale(0.7f);
    scaleInp->setCommonFilter(CommonFilter::Float);
    scaleInp->setCallback([this](const std::string& str)
    {
        currentConfig.scale = utils::numFromString<float>(str).unwrapOr(currentConfig.scale);
    });

    opacityInp = TextInput::create(160, "Opacity", "bigFont.fnt");
    opacityInp->setAnchorPoint(ccp(0, 0.5f));
    opacityInp->setScale(0.7f);
    opacityInp->setCommonFilter(CommonFilter::Float);
    opacityInp->setCallback([this](const std::string& str)
    {
        currentConfig.opacity = utils::numFromString<float>(str).unwrapOr(currentConfig.opacity);
    });

    cheatIndicatorToggler = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 1.0f);
    noclipOnlyToggler = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 1.0f);

    offsetXInp = TextInput::create(60, "X", "bigFont.fnt");
    offsetXInp->setAnchorPoint(ccp(0, 0.5f));
    offsetXInp->setScale(0.7f);
    offsetXInp->setCommonFilter(CommonFilter::Float);
    offsetXInp->setCallback([this](const std::string& str)
    {
        currentConfig.offset.x = utils::numFromString<float>(str).unwrapOr(currentConfig.offset.x);
    });

    offsetYInp = TextInput::create(60, "Y", "bigFont.fnt");
    offsetYInp->setAnchorPoint(ccp(0, 0.5f));
    offsetYInp->setScale(0.7f);
    offsetYInp->setCommonFilter(CommonFilter::Float);
    offsetYInp->setCallback([this](const std::string& str)
    {
        currentConfig.offset.y = utils::numFromString<float>(str).unwrapOr(currentConfig.offset.y);
    });

    pages[0]->addChildAtPosition(anchorMenu, Anchor::TopRight, ccp(-100, -100));
    pages[0]->addChildAtPosition(nameInp, Anchor::TopLeft, ccp(0, -100));
    pages[0]->addChildAtPosition(scaleInp, Anchor::TopLeft, ccp(0, -130));
    pages[0]->addChildAtPosition(opacityInp, Anchor::TopLeft, ccp(0, -160));
    pages[0]->addChildAtPosition(cheatIndicatorToggler, Anchor::TopLeft, ccp(50, -160));
    pages[0]->addChildAtPosition(noclipOnlyToggler, Anchor::TopLeft, ccp(50, -180));
    pages[0]->addChildAtPosition(offsetXInp, Anchor::Right, ccp(-90, -50));
    pages[0]->addChildAtPosition(offsetYInp, Anchor::Right, ccp(-50, -50));
}

void SetupLabelConfigUI::createPage2()
{
    formatInp = TextInput::create(450, "Format", "bigFont.fnt");
    formatInp->setAnchorPoint(ccp(0.5f, 0.5f));
    formatInp->setScale(0.7f);
    formatInp->setCommonFilter(CommonFilter::Any);
    formatInp->setCallback([this](const std::string& str)
    {
        currentConfig.formatString = str;
    });

    pages[1]->addChildAtPosition(formatInp, Anchor::Center);
}

void SetupLabelConfigUI::createPage3()
{

}

void SetupLabelConfigUI::createPages()
{
    std::vector<std::string> pageNames = { "General", "Format", "Events" };

    for (size_t i = 0; i < pageNames.size(); i++)
    {
        auto pg = CCMenu::create();
        pg->ignoreAnchorPointForPosition(false);
        pg->setContentSize(m_size);
        pg->setTag(i);
        pg->setZOrder(2);
        pg->setAnchorPoint(ccp(0.5f, 0.5f));
        pages.push_back(pg);

        m_mainLayer->addChildAtPosition(pg, Anchor::Center);

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