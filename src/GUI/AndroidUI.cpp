#include "AndroidUI.hpp"
#include "AndroidBall.hpp"
#include "../Client/ExtensionManager.hpp"
#include "Modules/FavouritesTab.hpp"
#include "Modules/SearchBox.hpp"
#include "../Client/SubCategoryNode.hpp"
#include "Modules/DisableOpenInLevel.hpp"
#include "BlurLayer.hpp"

bool AndroidUI::setup()
{
    rt = CCRenderTexture::create(getContentWidth(), getContentHeight());
    rt->getSprite()->setBlendFunc(this->getBlendFunc());

    drawOpacity = CCLayerColor::create();
    drawOpacity->setOpacity(255);
    drawOpacity->setVisible(false);
    this->addChild(drawOpacity);

    bg = BackgroundSprite::create();
    bg->setContentSize(m_size);
    bg->setAnchorPoint(ccp(0, 0));
    bg->updateLayout();

    m_bgSprite->setVisible(false);
    m_buttonMenu->setVisible(false);

    auto backMenu = CCMenu::create();
    backMenu->setPosition(ccp(24, CCDirector::get()->getWinSize().height - 23));
    
    auto backBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(AndroidUI::onClose));
    backMenu->addChild(backBtn);

    populateModules();
    populateTabs();

    m_mainLayer->addChild(bg, -1);
    m_mainLayer->setZOrder(80085);
    this->addChild(backMenu);

    auto e = QOLModUIOpenEvent(this);
    e.post();

    this->runAnimation((MenuAnimation)Mod::get()->getSavedValue<int>("menu-animation", (int)MenuAnimation::Scale));

    return true;
}

void AndroidUI::populateModules()
{
    categoryMenu = CCNode::create();
    categoryMenu->setContentSize(ccp(475, 280));
    categoryMenu->setID("category-menu");

    for (auto module : Module::moduleMap)
    {
        if (module->getParent() || module->getCategory().empty())
            continue;

        if (utils::string::split(module->getCategory(), "/").size() > 1)
        {
            auto cat2 = utils::string::split(module->getCategory(), "/")[0];

            if (!categories.contains(cat2))
            {
                auto cat = CategoryNode::getNode(cat2);
                cat->setID(cat2);
                categories.emplace(cat2, cat);

                categoryMenu->addChildAtPosition(cat, Anchor::Right, ccp(-10, 0));
            }

            if (auto sub = typeinfo_cast<SubCategoryNode*>(categories[cat2]))
            {
                sub->addModule(module, utils::string::split(module->getCategory(), "/")[1]);
            }

            continue;
        }

        if (!categories.contains(module->getCategory()))
        {
            auto cat = CategoryNode::getNode(module->getCategory());
            cat->setID(module->getCategory());
            categories.emplace(module->getCategory(), cat);

            categoryMenu->addChildAtPosition(cat, Anchor::Right, ccp(-10, 0));
        }

        categories[module->getCategory()]->addModule(module);
    }

    m_mainLayer->addChild(categoryMenu);
}

void AndroidUI::populateTabs()
{
    auto bg = CCScale9Sprite::create("square02b_small.png");
    bg->setContentSize(ccp(110, m_size.height - 10 * 2) / 0.5f);
    bg->setAnchorPoint(ccp(0, 0.5f));
    bg->setScale(0.5f);
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);

    tabsMenu = CCMenu::create();
    tabsMenu->setContentSize((bg->getContentSize() / 2) + ccp(0, -10));
    tabsMenu->setAnchorPoint(ccp(0, 0.5f));
    tabsMenu->ignoreAnchorPointForPosition(false);
    tabsMenu->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(3.5f));
    tabsMenu->getLayout()->ignoreInvisibleChildren(true);
    
    m_mainLayer->addChildAtPosition(bg, Anchor::Left, ccp(10, 0));
    m_mainLayer->addChildAtPosition(tabsMenu, Anchor::Left, ccp(10 + 5, 0));

    for (auto category : categoryOrders)
    {
        if (category == "spacer")
        {
            for (auto category : ExtensionManager::get()->getCategories())
            {
                addTab(category.name, fmt::format("{}/{}", category.modID, category.name), category.sprite);
            }

            tabsMenu->addChild(geode::SpacerNode::create());

            // TODO: Bottom row
            /*
            bottomTabsContainer = CCMenu::create();
            bottomTabsContainer->ignoreAnchorPointForPosition(false);
            bottomTabsContainer->setContentSize(ccp(100, 20));
            bottomTabsContainer->setLayout(AxisLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Even));
            tabsMenu->addChild(bottomTabsContainer);
            */

            continue;
        }

        if (!categories.contains(category))
        {
            // meowwww
            auto cat = CategoryNode::getNode(category);
            cat->setID(category);

            categories.emplace(category, cat);
            categoryMenu->addChildAtPosition(cat, Anchor::Right, ccp(-10, 0));
        }

        addTab(category, category, fmt::format("{}{}.png", ""_spr, utils::string::toLower(category)));
    }

    if (bottomTabsContainer)
    {
        bottomTabsContainer->updateLayout();
    }

    tabsMenu->updateLayout();
    updateTabs();
}

void AndroidUI::addTab(std::string name, std::string id, std::string sprite)
{
    auto sprNormal = CategoryTabSprite::create(CategoryTabType::Text, name, sprite);
    auto sprHeld = CategoryTabSprite::create(CategoryTabType::Text, name, sprite);
    sprHeld->updateSelection(CategorySelectionType::Hovered);

    sprNormal->setContentSize(ccp(100, 20));
    sprHeld->setContentSize(ccp(100, 20));

    if (bottomTabsContainer)
    {
        sprNormal->label->setString("");
        sprNormal->setContentSize(ccp((100 - 5 * 2) / 3, 20));

        sprHeld->label->setString("");
        sprHeld->setContentSize(ccp((100 - 5 * 2) / 3, 20));
    }

    auto btn = CCMenuItemSpriteExtra::create(sprNormal, this, menu_selector(AndroidUI::onSelectTab));
    btn->setID(id);
    btn->setSelectedImage(sprHeld);
    btn->m_scaleMultiplier = 1;

    sprNormal->setAnchorPoint(ccp(0, 0));
    sprHeld->setPosition(btn->getContentSize() / 2);

    if (bottomTabsContainer)
        bottomTabsContainer->addChild(btn);
    else
        tabsMenu->addChild(btn);

    categoryBtns[id] = btn;
    categorySprs[id] = sprNormal;
}

void AndroidUI::updateTabs()
{
    for (auto btn : categoryBtns)
    {
        btn.second->setEnabled(selectedCategory != btn.first);
    }
    
    for (auto spr : categorySprs)
    {
        spr.second->updateSelection(selectedCategory != spr.first ? CategorySelectionType::Deselected : CategorySelectionType::Selected);
    }

    for (auto category : categories)
    {
        category.second->setVisible(category.first == selectedCategory);
    }

    categoryBtns["Search"]->setVisible(SearchBox::get()->getRealEnabled());
    categoryBtns["Favourites"]->setVisible(FavouritesTab::get()->getRealEnabled());
    tabsMenu->updateLayout();
}

void AndroidUI::onSelectTab(CCObject* sender)
{
    selectedCategory = static_cast<CCNode*>(sender)->getID();

    updateTabs();
}

AndroidUI* AndroidUI::create()
{
    auto pRet = new AndroidUI();

    if (pRet && pRet->initAnchored(475.f, 280.f))
    {
        PlatformToolbox::showCursor();
        instance = pRet;
        pRet->m_noElasticity = true;

        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AndroidUI* AndroidUI::addToScene()
{
    if (DisableOpenInLevel::get()->getRealEnabled())
    {
        if (PlayLayer::get() && !PlayLayer::get()->m_isPaused)
            return nullptr;
    }

    auto pRet = create();
    pRet->show();

    return pRet;
}

AndroidUI* AndroidUI::get()
{
    return instance;
}

AndroidUI::~AndroidUI()
{
    instance = nullptr;
}

void AndroidUI::runAnimation(MenuAnimation anim)
{
    auto winSize = CCDirector::get()->getWinSize();
    auto moveToMid = CCSequence::create(CCDelayTime::create(0.1f), CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2), 0.8f), nullptr);

    switch (anim)
    {
        case MenuAnimation::None:
            return;

        case MenuAnimation::FromTop:
            m_mainLayer->setPosition(ccp(winSize.width / 2, winSize.height + m_size.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::FromBottom:
            m_mainLayer->setPosition(ccp(winSize.width / 2, -m_size.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::FromLeft:
            m_mainLayer->setPosition(ccp(-m_size.width / 2, winSize.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::FromRight:
            m_mainLayer->setPosition(ccp(winSize.width + m_size.width / 2, winSize.height / 2));

            m_mainLayer->runAction(moveToMid);
            return;

        case MenuAnimation::Scale:
            m_mainLayer->setScale(0);

            m_mainLayer->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f));
            return;

        case MenuAnimation::FadeIn:
            drawOpacity->setOpacity(0);

            drawOpacity->runAction(CCEaseOut::create(CCFadeTo::create(0.25f, 255), 2));
            return;
    }
}

void AndroidUI::close()
{
    if (PlayLayer::get() && !PlayLayer::get()->m_isPaused && !PlayLayer::get()->m_levelEndAnimationStarted && !GameManager::sharedState()->getGameVariable("0024"))
        PlatformToolbox::hideCursor();

    this->onClose(nullptr);
}

void AndroidUI::visit()
{
    AndroidBall::get()->visit();

    // for an animation i was making, but i couldnt get clipping to work right

    if (drawOpacity->getOpacity() == 255)
        return geode::Popup<>::visit();

    auto op = getOpacity();
    this->setOpacity(0);

    rt->beginWithClear(0, 0, 0, op / 255.0f);
    geode::Popup<>::visit();
    rt->end();

    this->setOpacity(op);

    rt->setPosition(getContentSize() / 2);
    rt->getSprite()->setOpacity(drawOpacity->getOpacity());
    rt->visit();
}