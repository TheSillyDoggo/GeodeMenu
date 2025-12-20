#include "SetupLabelConfigUI.hpp"
#include "../LabelManager.hpp"
#include "../../GUI/CategoryTabSprite.hpp"
#include "../../GUI/BlurLayer.hpp"
#include "LabelEventCell.hpp"

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
    this->addChild(CCBlurLayer::create(), -3);
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

    updateEventsUI();
}

void SetupLabelConfigUI::createPage1()
{
    auto info = InfoAlertButton::create("General Label Options", "These are options for the label", 0.65f);
    pages[0]->addChildAtPosition(info, Anchor::TopRight, ccp(-16, -16));

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
    auto info = InfoAlertButton::create("Format Label", "Format label help text", 0.65f);
    pages[1]->addChildAtPosition(info, Anchor::TopRight, ccp(-16, -16));

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
    auto info = InfoAlertButton::create("Label Events", "Change the colour of the label when an event happens in-game.\nChanging the time to -1 (inf)\nwill make the action not fade back to white.\nThis can allow for events to set the colour without it fading back", 0.65f);
    pages[2]->addChildAtPosition(info, Anchor::TopRight, ccp(-16, -16));

    auto rightBG = CCScale9Sprite::create("square02b_small.png");
    rightBG->setContentSize(ccp(75, this->getContentHeight() - 170) / 0.5f);
    rightBG->setScale(0.55f);
    rightBG->setAnchorPoint(ccp(1, 0.5f));
    rightBG->setColor(ccc3(0, 0, 0));
    rightBG->setOpacity(100);

    auto leftBG = CCScale9Sprite::create("square02b_small.png");
    leftBG->setContentSize(ccp(210, this->getContentHeight() - 170) / 0.5f);
    leftBG->setScale(0.55f);
    leftBG->setAnchorPoint(ccp(0, 0.5f));
    leftBG->setColor(ccc3(0, 0, 0));
    leftBG->setOpacity(100);

    auto addNewTitle = CCLabelBMFont::create("Add New", "bigFont.fnt");
    addNewTitle->setScale(0.5f);

    auto addNewLine = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    addNewLine->setRotation(90);
    addNewLine->setScaleY(0.9f);

    eventsNone = CCLabelBMFont::create("No events added yet", "bigFont.fnt");
    eventsNone->setScale(0.45f);

    eventScroll = ScrollLayer::create(leftBG->getScaledContentSize());
    eventScroll->m_peekLimitTop = 15;
    eventScroll->m_peekLimitBottom = 15;

    pages[2]->addChildAtPosition(leftBG, Anchor::Left, ccp(20, 5));
    pages[2]->addChildAtPosition(eventScroll, Anchor::Left, ccp(20, 5 - eventScroll->getContentHeight() / 2));
    pages[2]->addChildAtPosition(eventsNone, Anchor::Left, ccp(20 + eventScroll->getContentWidth() / 2, 5));
    pages[2]->addChildAtPosition(rightBG, Anchor::Right, ccp(-20, 5));
    pages[2]->addChildAtPosition(addNewTitle, Anchor::TopRight, ccp(-20 - rightBG->getScaledContentWidth() / 2, -50));
    pages[2]->addChildAtPosition(addNewLine, Anchor::TopRight, ccp(-20 - rightBG->getScaledContentWidth() / 2, -45 - 18));

    for (int i = 0; i < 7; i++)
    {
        auto eventType = (LabelEventType)i;
        std::string lbl = "";

        switch (eventType)
        {
            case LabelEventType::PlayerTookDamage:
                lbl = "Player Took Damage";
                break;

            case LabelEventType::ClickStarted:
                lbl = "Click Started";
                break;
            case LabelEventType::ClickEnded:
                lbl = "Click Ended";
                break;
            case LabelEventType::P1ClickStarted:
                lbl = "P1 Click Started";
                break;
            case LabelEventType::P1ClickEnded:
                lbl = "P1 Click Ended";
                break;
            case LabelEventType::P2ClickStarted:
                lbl = "P2 Click Started";
                break;
            case LabelEventType::P2ClickEnded:
                lbl = "P2 Click Ended";
                break;
        }

        auto spr = ButtonSprite::create(lbl.c_str(), 90, 90, 0.8f, false, "goldFont.fnt", "GJ_button_04.png", 30);
        spr->setScale(0.7f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SetupLabelConfigUI::onAddEvent));
        btn->m_scaleMultiplier = 1.1f;
        btn->setTag(i);

        pages[2]->addChildAtPosition(btn, Anchor::TopRight, ccp(-20 - rightBG->getScaledContentWidth() / 2, -45 - 18 - 18) + ccp(0, -25 * i));
    }

    updateEventsUI();
}

void SetupLabelConfigUI::updateEventsUI()
{
    eventScroll->m_contentLayer->removeAllChildren();

    float cellHeight = 50;
    float height = std::max<float>(cellHeight * currentConfig.events.size(), eventScroll->getContentHeight());
    eventScroll->setTouchEnabled(height != eventScroll->getContentHeight());

    eventScroll->m_contentLayer->setContentHeight(height);

    int i = 0;
    for (auto event : currentConfig.events)
    {
        auto cell = LabelEventCell::create(ccp(eventScroll->getContentWidth(), cellHeight), event, this, i);
        cell->setPositionY(height - cellHeight * (i + 1));
        eventScroll->m_contentLayer->addChild(cell);

        i++;
    }

    eventsNone->setVisible(i == 0);
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

void SetupLabelConfigUI::onAddEvent(CCObject* sender)
{
    LabelEvent event;
    event.type = (LabelEventType)sender->getTag();

    currentConfig.events.push_back(event);

    updateEventsUI();
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