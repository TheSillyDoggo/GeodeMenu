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

#include "FloatingButton/FloatingUIManager.hpp"
#include "EasyBG.hpp"
#include <BlurAPI.hpp>

bool AndroidUI::setup()
{
    instance = this;
    this->scheduleUpdate();

    rt = CCRenderTexture::create(getContentWidth(), getContentHeight(), kCCTexture2DPixelFormat_RGBA8888, GL_DEPTH24_STENCIL8);
    rt->getSprite()->setBlendFunc(this->getBlendFunc());

    drawOpacity = CCLayerColor::create();
    drawOpacity->setOpacity(255);
    drawOpacity->setVisible(false);
    this->addChild(drawOpacity);

    bottomLeft = VersionInfoNode::create(VersionInfoType::GeodeAndGD);
    bottomLeft->setAnchorPoint(ccp(0, 0));
    bottomLeft->setPosition(ccp(5, 2.5f));
    bottomLeft->setColor(ccc3(100, 100, 100));
    bottomLeft->setScale(0.5f);
    this->addChild(bottomLeft);

    bottomRight = VersionInfoNode::create(VersionInfoType::QOLMod);
    bottomRight->setAnchorPoint(ccp(1, 0));
    bottomRight->setPosition(ccp(CCDirector::get()->getWinSize().width - 5, 2.5f));
    bottomRight->setColor(ccc3(100, 100, 100));
    bottomRight->setScale(0.5f);
    this->addChild(bottomRight);
    

    bg = BackgroundSprite::create();
    bg->setContentSize(m_size);
    bg->setAnchorPoint(ccp(0, 0));
    bg->updateLayout();

    m_bgSprite->setVisible(false);
    m_buttonMenu->setVisible(false);

    auto backMenu = CCMenu::create();
    backMenu->setPosition(ccp(24, CCDirector::get()->getWinSize().height - 23));
    
    backBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), this, menu_selector(AndroidUI::onClose));
    backMenu->addChild(backBtn);

    catMenu = CategorySelectMenu::create();
    catMenu->setAnchorPoint(ccp(0, 0.5f));
    catMenu->setContentSize(ccp(105, 255));

    populateModules();
    populateTabs();

    m_mainLayer->addChildAtPosition(catMenu, Anchor::Left, ccp(10 + 2.5f, 0));
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

    catMenu->setSelectedCategory(selectedCategory);
    catMenu->setCallback([this](std::string tab)
    {
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

void AndroidUI::runAnimation(MenuAnimation anim)
{
    auto winSize = CCDirector::get()->getWinSize();
    auto moveToMid = RealtimeAction::create(CCSequence::create(CCDelayTime::create(0.1f), CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2), 0.8f), nullptr));
    auto fadeIn = RealtimeAction::create(CCSequence::create(CCDelayTime::create(0.1f), CCFadeTo::create(0.25f, 255), nullptr));
    auto fadeInVersion = RealtimeAction::create(CCSequence::create(CCFadeTo::create(0, 0), CCDelayTime::create(0.1f), CCFadeTo::create(0.25f, 150), nullptr));
    auto fadeInVersion2 = RealtimeAction::create(CCSequence::create(CCFadeTo::create(0, 0), CCDelayTime::create(0.1f), CCFadeTo::create(0.25f, 150), nullptr));
    auto fadeInVersion3 = RealtimeAction::create(CCSequence::create(CCFadeTo::create(0, 0), CCDelayTime::create(0.1f), CCFadeTo::create(0.25f, 150), nullptr));
    auto fadeInBG = RealtimeAction::create(CCSequence::create(CCFadeTo::create(0, 0), CCDelayTime::create(0.1f), CCFadeTo::create(0.14f, 150), nullptr));

    backBtn->stopAllActions();
    drawOpacity->stopAllActions();
    m_mainLayer->stopAllActions();
    m_mainLayer->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    m_mainLayer->setScale(1.0f);
    backBtn->setOpacity(255);
    bottomLeft->setOpacity(150);
    bottomRight->setOpacity(150);
    this->setOpacity(150);

    if (BlurAPI::isBlurAPIEnabled() && BlurMenuBG::get()->getRealEnabled() && anim == MenuAnimation::FadeIn)
        anim = MenuAnimation::None;

    switch (anim)
    {
        case MenuAnimation::None:
            return;

        case MenuAnimation::FromTop:
            backBtn->setOpacity(0);
            backBtn->runAction(fadeIn);

            m_mainLayer->setPosition(ccp(winSize.width / 2, winSize.height + m_size.height / 2));

            m_mainLayer->runAction(moveToMid);
            bottomLeft->runAction(fadeInVersion);
            bottomRight->runAction(fadeInVersion2);
            this->setOpacity(0);
            this->runAction(fadeInBG);
            return;

        case MenuAnimation::FromBottom:
            backBtn->setOpacity(0);
            backBtn->runAction(fadeIn);

            m_mainLayer->setPosition(ccp(winSize.width / 2, -m_size.height / 2));

            m_mainLayer->runAction(moveToMid);
            bottomLeft->runAction(fadeInVersion);
            bottomRight->runAction(fadeInVersion2);
            this->setOpacity(0);
            this->runAction(fadeInBG);
            return;

        case MenuAnimation::FromLeft:
            backBtn->setOpacity(0);
            backBtn->runAction(fadeIn);

            m_mainLayer->setPosition(ccp(-m_size.width / 2, winSize.height / 2));

            m_mainLayer->runAction(moveToMid);
            bottomLeft->runAction(fadeInVersion);
            bottomRight->runAction(fadeInVersion2);
            this->setOpacity(0);
            this->runAction(fadeInBG);
            return;

        case MenuAnimation::FromRight:
            backBtn->setOpacity(0);
            backBtn->runAction(fadeIn);

            m_mainLayer->setPosition(ccp(winSize.width + m_size.width / 2, winSize.height / 2));

            m_mainLayer->runAction(moveToMid);
            bottomLeft->runAction(fadeInVersion);
            bottomRight->runAction(fadeInVersion2);
            this->setOpacity(0);
            this->runAction(fadeInBG);
            return;

        case MenuAnimation::Scale:
            backBtn->setOpacity(0);
            backBtn->runAction(fadeIn);

            m_mainLayer->setScale(0);

            m_mainLayer->runAction(RealtimeAction::create(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f)));
            bottomLeft->runAction(fadeInVersion);
            bottomRight->runAction(fadeInVersion2);
            this->setOpacity(0);
            this->runAction(fadeInBG);
            return;

        case MenuAnimation::FadeIn:
            drawOpacity->setOpacity(0);
            drawOpacity->runAction(RealtimeAction::create(CCEaseOut::create(CCFadeTo::create(0.25f, 255), 2)));
            this->setOpacity(0);
            this->runAction(fadeInBG);
            return;
    }
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

    // ?
    /*if (SearchOnKeyPress::get()->getRealEnabled())
    {
        if (selectedCategory != "Search")
        {
            selectedCategory = "Search";

            auto inp = categories["Search"]->getChildByType<BetterInputNode*>(0);

            inp->selectInput(true);

            if (inp->getString().empty())
            {
                if (auto ch = CCKeyboardDispatcher::get()->keyToString(key))
                {
                    inp->setString(" ");
                }
            }

            updateTabs();
        }
    }*/

    selectedCategory = old;
}

void AndroidUI::visit()
{
    FloatingUIManager::get()->visit();
    AndroidBall::get()->visit();    

    /*if (selectedCategory != "Search" && categories["Search"]->isVisible())
    {
        if (categories["Search"]->getChildByType<BetterInputNode*>(0)->getString().empty())
        {
            categories["Search"]->getChildByType<BetterInputNode*>(0)->selectInput(false);

            updateTabs();
        }
    }*/

    // for an animation i was making, but i couldnt get clipping to work right

    if (drawOpacity->getOpacity() == 255)
        return PopupBase::visit();

    auto op = getOpacity();
    this->setOpacity(0);

    rt->beginWithClear(0, 0, 0, op / 255.0f);
    PopupBase::visit();
    rt->end();

    this->setOpacity(op);

    rt->setPosition(getContentSize() / 2);
    rt->getSprite()->setOpacity(drawOpacity->getOpacity());
    rt->visit();
}