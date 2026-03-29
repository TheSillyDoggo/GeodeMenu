#include "CategorySelectMenu.hpp"

using namespace qolmod;

void CategorySelectMenu::setInset(float inset)
{
    this->inset = inset;
}

float CategorySelectMenu::getInset()
{
    return inset;
}

void CategorySelectMenu::addCategory(std::string title, std::string sprite, std::string id, CategoryType type)
{
    auto sprOff = CategoryTabSprite::create(CategoryTabType::Image, title, sprite);
    auto sprSelected = CategoryTabSprite::create(CategoryTabType::Image, title, sprite);
    sprOff->updateSelection(CategorySelectionType::Deselected);
    sprSelected->updateSelection(CategorySelectionType::Hovered);

    auto btn = Button::create(sprOff, sprSelected, this, menu_selector(CategorySelectMenu::onSelect));
    btn->setID(id);
    btn->setLayout(CopySizeLayout::create()->add(sprOff)->add(sprSelected));
    btn->setContentSize(ccp(105, 20));
    btn->m_scaleMultiplier = 1;

    sprSelected->setAnchorPoint(ccp(0.5f, 0.5f));

    if (type == CategoryType::Bottom)
        bottomBtns->addChild(btn);
    else
        standardBtns->addChild(btn);

    btn->setAnchorPoint(ccp(0, 0.5f));

    buttons.push_back(btn);

    if (buttons.size() == 1)
        setSelectedCategory(id);

    update();
}

void CategorySelectMenu::onSelect(CCObject* sender)
{
    setSelectedCategory(static_cast<CCNode*>(sender)->getID());

    if (callback)
        callback(selectedTab);
}

bool CategorySelectMenu::init()
{
    if (!CCMenu::init())
        return false;

    ignoreAnchorPointForPosition(false);
    setAnchorPoint(ccp(0.5f, 0.5f));
    setContentSize(ccp(0, 0));

    scroll = ScrollLayer::create(ccp(100, 100));
    scroll->setID("category-select-menu-scroll");
    scroll->setUserObject("MouseDispatcherExt"_spr, CCNode::create());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;

    scrollbar = BetterScrollbar::create(scroll);
    scrollbar->setAnchorPoint(ccp(1, 1));
    scrollbar->setUseDynamicHandle(true);

    bottomBtns = CCMenuExt::create();
    bottomBtns->setAnchorPoint(ccp(0, 0));
    bottomBtns->ignoreAnchorPointForPosition(false);
    bottomBtns->setPosition(ccp(0, 0));
    bottomBtns->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::Start)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(2.5f)->setAutoGrowAxis(true));
    static_cast<AxisLayout*>(bottomBtns->getLayout())->ignoreInvisibleChildren(true);
    bottomBtns->setTouchEnabled(true);

    standardBtns = CCMenuExt::create();
    standardBtns->setAnchorPoint(ccp(0, 0));
    standardBtns->ignoreAnchorPointForPosition(false);
    standardBtns->setPosition(ccp(0, 0));
    standardBtns->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(2.5f)->setAutoGrowAxis(true));
    static_cast<AxisLayout*>(standardBtns->getLayout())->ignoreInvisibleChildren(true);
    standardBtns->setTouchEnabled(true);
    standardBtns->setLinkedRect(scroll);

    this->addChild(scroll);
    this->addChild(scrollbar);
    this->addChild(bottomBtns);
    scroll->m_contentLayer->addChild(standardBtns);
    return true;
}

bool CategorySelectMenu::shouldScrollbarShow()
{
    return scroll->m_contentLayer->getContentHeight() > scroll->getContentHeight();
}

void CategorySelectMenu::visit()
{
    update();

    CCMenu::visit();
}

void CategorySelectMenu::update()
{
    auto sc = shouldScrollbarShow();

    scrollbar->setPosition(getContentSize());
    scrollbar->setVisible(sc);
    scroll->setContentSize(ccp(getContentWidth() - (sc ? scrollbar->getContentWidth() : 0) + 20, getContentHeight() - bottomBtns->getContentHeight() - inset));
    scroll->setPositionX(-20 / 2);
    scroll->setPositionY(bottomBtns->getContentHeight() + inset);
    scroll->m_contentLayer->setContentSize(ccp(scroll->getContentWidth(), standardBtns->getContentHeight()));

    scroll->m_contentLayer->setPositionX(20 / 2);
    if (!sc)
        scroll->m_contentLayer->setPositionY(scroll->getContentHeight() - scroll->m_contentLayer->getContentHeight());

    scroll->setTouchEnabled(sc);
    scroll->setMouseEnabled(sc);

    updateButtonSizes();
}

void CategorySelectMenu::updateButtonSizes()
{
    auto width = getContentWidth();

    for (auto btn : buttons)
    {
        if (btn->getParent() == bottomBtns)
            btn->setContentSize(ccp(width, 20));
        else
            btn->setContentSize(ccp(width - (shouldScrollbarShow() ? scrollbar->getContentWidth() + inset : 0), 20));

        btn->updateLayout();
    }

    standardBtns->updateLayout();
    bottomBtns->updateLayout();
}

void CategorySelectMenu::setSelectedCategory(std::string id)
{
    selectedTab = id;

    for (auto button : buttons)
    {
        bool sel = id == button->getID();

        button->setEnabled(!sel);
        static_cast<CategoryTabSprite*>(button->getNormalImage())->updateSelection(sel ? CategorySelectionType::Selected : CategorySelectionType::Deselected);
    }
}

void CategorySelectMenu::setCallback(std::function<void(std::string)> callback)
{
    this->callback = std::move(callback);
}

Button* CategorySelectMenu::getButton(std::string id)
{
    for (auto button : buttons)
    {
        if (button->getID() == id)
            return button;
    }

    return nullptr;
}

bool CategorySelectMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return false;
    if (bottomBtns->ccTouchBegan(touch, event))
    {
        touchInitialScrollY = scroll->m_contentLayer->getPositionY();
        touchCancelled = false;

        bottomBtns->ccTouchMoved(touch, event);
        standardBtns->ccTouchBegan(touch, event);
        return true;
    }

    if (standardBtns->ccTouchBegan(touch, event))
    {
        touchInitialScrollY = scroll->m_contentLayer->getPositionY();
        touchCancelled = false;

        bottomBtns->ccTouchBegan(touch, event);
        return true;
    }

    return false;
}

void CategorySelectMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (std::abs<float>(scroll->m_contentLayer->getPositionY() - touchInitialScrollY) > 5)
    {
        if (bottomBtns->m_pSelectedItem)
            bottomBtns->m_pSelectedItem->unselected();

        if (standardBtns->m_pSelectedItem)
            standardBtns->m_pSelectedItem->unselected();

        bottomBtns->m_eState = kCCMenuStateWaiting;
        standardBtns->m_eState = kCCMenuStateWaiting;

        bottomBtns->m_pSelectedItem = nullptr;
        standardBtns->m_pSelectedItem = nullptr;

        touchCancelled = true;
    }

    if (touchCancelled == true)
        return;

    bottomBtns->ccTouchMoved(touch, event);
    standardBtns->ccTouchMoved(touch, event);
}

void CategorySelectMenu::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (touchCancelled == true)
        return;

    bottomBtns->ccTouchEnded(touch, event);
    standardBtns->ccTouchEnded(touch, event);
}

void CategorySelectMenu::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    bottomBtns->ccTouchCancelled(touch, event);
    standardBtns->ccTouchCancelled(touch, event);
}