#include "AndroidUI.hpp"
#include "AndroidBall.hpp"
#include "../Client/ExtensionManager.hpp"
#include "Modules/FavouritesTab.hpp"
#include "Modules/SearchBox.hpp"
#include "../Client/SubCategoryNode.hpp"
#include "Modules/DisableOpenInLevel.hpp"
#include "Modules/BlurBackground.hpp"
#include "../Utils/RealtimeAction.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "BetterInputNode.hpp"
#include "Categories/ExtensionCategory.hpp"
#include "FloatingButton/FloatingUIManager.hpp"
#include "EasyBG.hpp"
#include <BlurAPI.hpp>

using namespace qolmod;

bool AndroidUI::setup()
{
    instance = this;

    bottomLeft = VersionInfoNode::create(VersionInfoType::GeodeAndGD);
    bottomLeft->setAnchorPoint(ccp(0, 0));
    bottomLeft->setColor(ccc3(100, 100, 100));
    bottomLeft->setScale(0.5f);
    this->addChildAtPosition(bottomLeft, Anchor::BottomLeft, ccp(5, 2.5f));

    bottomRight = VersionInfoNode::create(VersionInfoType::QOLMod);
    bottomRight->setAnchorPoint(ccp(1, 0));
    bottomRight->setColor(ccc3(100, 100, 100));
    bottomRight->setScale(0.5f);
    this->addChildAtPosition(bottomRight, Anchor::BottomRight, ccp(-5, 2.5f));
    

    bg = BackgroundSprite::create();
    bg->setContentSize(m_size);
    bg->setAnchorPoint(ccp(0, 0));
    bg->updateLayout();

    m_bgSprite->setVisible(false);
    m_buttonMenu->setVisible(false);

    auto backMenu = CCMenu::create();
    
    backBtn = Button::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(AndroidUI::onClose));
    backMenu->addChild(backBtn);

    catMenu = CategorySelectMenu::create();
    catMenu->setAnchorPoint(ccp(0, 0.5f));
    catMenu->setContentSize(ccp(105, 255));

    populateModules();
    populateTabs();

    m_mainLayer->addChildAtPosition(catMenu, Anchor::Left, ccp(10 + 2.5f, 0));
    m_mainLayer->addChild(bg, -1);
    m_mainLayer->setZOrder(80085);
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(24, -23));

    // auto e = QOLModUIOpenEvent(this);
    // e.post();

    this->playAnimation((MenuAnimation)Mod::get()->getSavedValue<int>("menu-animation", (int)MenuAnimation::Scale));

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
    auto bg = EasyBG::create();
    bg->setContentSize(ccp(110, m_size.height - 10 * 2));
    bg->setAnchorPoint(ccp(0, 0.5f));
    m_mainLayer->addChildAtPosition(bg, Anchor::Left, ccp(10, 0));

    bool bottom = false;

    for (auto category : categoryOrders)
    {
        if (category == "spacer")
        {
            bottom = true; // so me core
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

        catMenu->addCategory(LocalisationManager::get()->getLocalisedString(fmt::format("categories/{}", utils::string::toLower(category))), fmt::format("{}{}.png", ""_spr, utils::string::toLower(category)), category, bottom ? CategoryType::Bottom : CategoryType::Standard);
    }

    for (qolmod::ext::CategoryData& cat : ExtensionManager::get()->getCategories())
    {
        if (!categories.contains(cat.categoryID))
        {
            auto cat2 = ExtensionCategory::create(&cat);
            cat2->setID(cat.categoryID);

            categories.emplace(cat.categoryID, cat2);
            categoryMenu->addChildAtPosition(cat2, Anchor::Right, ccp(-10, 0));
        }

        catMenu->addCategory(
            cat.displayName,
            cat.iconSpr,
            cat.categoryID,
            CategoryType::Extension
        );
    }

    catMenu->setSelectedCategory(selectedCategory);
    catMenu->setCallback([this](std::string tab)
    {
        categoryHistory.push_back(tab);
        selectedCategory = tab;
        updateTabs();
    });

    updateTabs();
}

void AndroidUI::updateTabs()
{
    for (auto category : categories)
    {
        category.second->setVisible(category.first == selectedCategory);
    }

    catMenu->getButton("Search")->setVisible(SearchBox::get()->getRealEnabled());
    catMenu->getButton("Favourites")->setVisible(FavouritesTab::get()->getRealEnabled());
}

AndroidUI* AndroidUI::create()
{
    auto pRet = new AndroidUI();

    if (pRet && pRet->initAnchored(475.f, 280.f))
    {
        PlatformToolbox::showCursor();
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
        if (PlayLayer::get() && !(PlayLayer::get()->m_isPaused || PlayLayer::get()->m_levelEndAnimationStarted))
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
    if (instance == this)
        instance = nullptr;
}

void AndroidUI::close()
{
    if (PlayLayer::get() && !PlayLayer::get()->m_isPaused && !PlayLayer::get()->m_levelEndAnimationStarted && !GameManager::sharedState()->getGameVariable("0024"))
        PlatformToolbox::hideCursor();

    this->onClose(nullptr);
    instance = nullptr;
}

void AndroidUI::keyDown(cocos2d::enumKeyCodes key, double timestamp)
{
    PopupBase::keyDown(key, timestamp);

    auto old = selectedCategory;

    selectedCategory = old;
}

void AndroidUI::visit()
{
    FloatingUIManager::get()->visit();
    AndroidBall::get()->visit();    

    PopupBase::visit();
}

void AndroidUI::switchTabTemp(std::string tab)
{
    if (!allow)
    {
        // allow = true;
        return;
    }

    auto old = selectedCategory;

    selectedCategory = tab;
    updateTabs();

    if (tab == "Search")
        categories["Search"]->getChildByType<BetterInputNode*>(0)->selectInput(true);

    selectedCategory = old;
}

bool AndroidUI::altClickBegan(int button, cocos2d::CCPoint point)
{
    switch (button)
    {
        case 4: // forward
            if (!categoryHistory.empty())
            {
                selectedCategory = categoryHistory[0];
                categoryHistory.erase(categoryHistory.begin());
            }
            return true;

        case 3: // back
            if (!categoryHistory.empty())
            {
                
            }
            return true;

        default:
            return false;
    }
}