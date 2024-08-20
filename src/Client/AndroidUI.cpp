#include "AndroidUI.h"
#include "../Utils/CCBlurLayer.hpp"

bool AndroidUI::setup()
{
    instance = this;
    m_mainLayer->setVisible(false);

    this->setTouchEnabled(false);
    this->setKeypadEnabled(true);
    this->setMouseEnabled(false);
    this->scheduleUpdate();
    this->setID("QOLModUI");
    this->runAction(CCFadeTo::create(0.5f, 100));

    if (Client::GetModuleEnabled("menu-bg-blur"))
    {
        auto blur = CCBlurLayer::create();
        blur->setID("blur-layer");
        blur->runAction(CCEaseIn::create(CCFadeTo::create(0.5f, 255), 2));
        this->addChild(blur);
    }

    auto backMenu = CCMenu::create();
    backMenu->ignoreAnchorPointForPosition(false);
    backMenu->setContentSize(ccp(0, 0));
    backMenu->setPositionX(0);
    backMenu->setPositionY(CCDirector::get()->getWinSize().height);
    backMenu->setID("back-menu");

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");

    #ifdef GEODE_IS_APPLE
    backSpr->runAction(CCFadeIn::create(0.5f));
    #else
    backSpr->runAction(CCSpeed::create(CCFadeIn::create(0.5f), 1.0f / SpeedhackTop::getAdjustedValue()));
    #endif

    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(AndroidUI::onClose));
    backBtn->setPosition(ccp(24, -23));
    backBtn->setSizeMult(1.15f);
    backMenu->addChild(backBtn);

    auto panel = getBGNode();
    
    auto windows = CCScale9Sprite::create("square02_small.png");
    windows->setOpacity(100);
    windows->setPosition(ccp(10, 10));
    windows->setContentSize(ccp(110, panel->getContentSize().height - 10 - 10));
    windows->setAnchorPoint(ccp(0, 0));
    windows->setID("windows-panel");

    auto windowsMenu = CCMenu::create();
    windowsMenu->setContentSize(ccp(windows->getContentSize().width, windows->getContentSize().height - 10));
    windowsMenu->setAnchorPoint(ccp(0, 0));
    windowsMenu->setPosition(ccp(5, 5));
    windowsMenu->ignoreAnchorPointForPosition(false);
    windowsMenu->setID("windows-menu");

    windowsMenu->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(3.5f));
    windows->addChild(windowsMenu);

    for (size_t i = 0; i < Client::instance->windows.size(); i++)
    {
        auto win = Client::instance->windows[i];
        auto tabSize = ccp(100, 20);

        auto normal = CategoryTabSprite::create(CategoryTabType::Text, win->name);
        normal->setContentSize(tabSize);

        auto hovered = CategoryTabSprite::create(CategoryTabType::Text, win->name);
        hovered->setContentSize(tabSize);
        hovered->updateSelection(CategorySelectionType::Hovered);

        auto button = CCMenuItemSpriteExtra::create(normal, hovered, this, menu_selector(AndroidUI::onPressTab));
        button->setTag(i);
        button->setSelectedImage(hovered); // this is required on everything other than ios?? wtf cocos
        button->setContentSize(tabSize);
        button->m_animationEnabled = false;
        button->setID(win->id);

        normal->setAnchorPoint(CCPointZero);
        hovered->setAnchorPoint(CCPointZero);
        hovered->setPosition(tabSize / 2);

        sprites.push_back(normal);
        windowsMenu->addChild(button);
    }

    updateTabs();

    for (size_t i = 0; i < Client::instance->windows.size(); i++)
    {
        auto menu = CCMenu::create();
        menu->setAnchorPoint(ccp(1, 0));
        menu->setPosition(ccp(475 - 15 + 5, 10));
        menu->setContentSize(ccp(340, panel->getContentSize().height - 10 - 10));
        menu->ignoreAnchorPointForPosition(false);

        menu->setID(Client::instance->windows[i]->id);

        Client::instance->windows[i]->cocosCreate(menu);
        
        pages.push_back(menu);

        panel->addChild(menu);
    }

    searchResultsPanel = getSearchPanel();
    panel->addChild(searchResultsPanel);

    windowsMenu->updateLayout();
    
    panel->addChild(windows);

    float height = 25;
    auto input = TextInput::create(100, "Search Mods");
    input->setMaxCharCount(20);
    input->setCommonFilter(CommonFilter::Any);
    input->setPosition(ccp(15, 28));
    input->setContentHeight(height);
    input->getInputNode()->setContentHeight(height);
    input->getBGSprite()->setContentHeight(height * 2);
    input->getBGSprite()->setPositionY(height / 2);
    input->getInputNode()->setPositionY(height / 2);
    input->getBGSprite()->setContentSize(input->getBGSprite()->getContentSize() / 0.5f);
    input->getBGSprite()->setScale(input->getBGSprite()->getScale() * 0.5f);
    input->getInputNode()->setAnchorPoint(ccp(0, 0));
    input->setAnchorPoint(ccp(0, 0.5f));
    input->setDelegate(this);
    input->getInputNode()->setID("IGNOREBYPASSES"_spr);
    input->setString("");
    input->setID("search-input");
    input->getInputNode()->m_cursor->setID("cursor");
    if (auto cursor = input->getInputNode()->m_cursor->getChildren()->objectAtIndex(0))
    {
        as<CCNode*>(cursor)->setID("cursor-char");
    }
    searchLabel = input->getInputNode()->m_placeholderLabel;

    panel->addChild(input);
    this->inputField = input;

    std::stringstream ver;
    ver << "Using version " << Mod::get()->getVersion().getMajor() << "." << Mod::get()->getVersion().getMinor() << "." << Mod::get()->getVersion().getPatch();

    auto versionText = CCLabelBMFont::create(ver.str().c_str(), "chatFont.fnt");
    versionText->setColor({0, 0, 0});
    versionText->setOpacity(100);
    versionText->setAnchorPoint(ccp(0.5f, 0));
    versionText->setScale(0.45f);
    versionText->setPosition(ccp(64, 13 + 8));
    //panel->addChild(versionText);

    auto devText = CCLabelBMFont::create("Mod Developed By TheSillyDoggo", "chatFont.fnt");
    devText->setColor({0, 0, 0});
    devText->setOpacity(100);
    devText->setAnchorPoint(ccp(0.5f, 0));
    devText->setScale(0.45f);
    devText->setPosition(ccp(64, 13));
    //panel->addChild(devText);

    goToPage(selectedTab);

    if (Client::GetModuleEnabled("npesta-width"))
    {
        panel->setPositionX(panel->getPositionX() + 5);
        panel->setContentSize(panel->getContentSize() + ccp(10, 0));
        as<CCNode*>(panel->getChildren()->objectAtIndex(0))->setPositionX(-5);
    }

    panel->runAction(getEnterAction(panel));

    this->addChild(panel);
    this->addChild(backMenu);
    return true;
}

CCNode* AndroidUI::getBGNode()
{
    int theme = Mod::get()->getSavedValue<int>("theme", 5);

    panel = CCScale9Sprite::create(fmt::format("GJ_square0{}.png", (theme < 0 ? 6 : theme)).c_str());
    panel->setContentSize(ccp(475, 280));
    panel->setID("panel");

    as<CCNode*>(panel->getChildren()->objectAtIndex(0))->setZOrder(-2);

    if (Loader::get()->getLoadedMod("thesillydoggo.gradientpages") && theme == -1)
    {
        auto gradient = CCLayerGradient::create(ccc4(0, 0, 0, 0), ccc4(0, 0, 0, 0));
        gradient->setContentSize(panel->getContentSize());
        gradient->setZOrder(-1);
        gradient->setID("gradient"_spr);

        if (Loader::get()->getLoadedMod("thesillydoggo.gradientpages")->getSettingValue<bool>("use-custom-colours"))
        {
            gradient->setStartColor(Loader::get()->getLoadedMod("thesillydoggo.gradientpages")->getSettingValue<ccColor3B>("primary-colour"));
            gradient->setEndColor(Loader::get()->getLoadedMod("thesillydoggo.gradientpages")->getSettingValue<ccColor3B>("secondary-colour"));
        }
        else
        {
            auto gm = GameManager::get();

            gradient->setStartColor(gm->colorForIdx(gm->m_playerColor.value()));
            gradient->setEndColor(gm->colorForIdx(gm->m_playerColor2.value()));
        }

        gradient->setStartOpacity(255);
        gradient->setEndOpacity(255);

        gradient->setPosition(CCDirector::get()->getWinSize() / 2);
        gradient->ignoreAnchorPointForPosition(false);

        if (Loader::get()->getLoadedMod("thesillydoggo.gradientpages")->getSettingValue<bool>("reverse-order"))
            gradient->setScaleY(-1);

        auto outline = CCScale9Sprite::createWithSpriteFrameName((std::string("thesillydoggo.gradientpages/") + std::string("square-outline.png")).c_str());
        outline->setPosition(panel->getContentSize() / 2);
        outline->setContentSize(panel->getContentSize());
        outline->setZOrder(1);
        outline->setID("outline"_spr);
        
        gradient->addChild(outline);
        panel->addChild(gradient);

        gradient->setAnchorPoint(ccp(0, 0));
        gradient->setPosition(ccp(0, 0));
    }

    if (theme == -2)
    {
        panel->setColor(ccc3(0, 0, 0));
        panel->setOpacity(175);

        auto out = CCScale9Sprite::create("GJ_square07.png");
        out->setContentSize(panel->getContentSize());
        out->setAnchorPoint(ccp(0, 0));
        out->setID("panel-outline");
        panel->addChild(out);
    }

    if (theme == -3)
    {
        if (auto spr = CCSprite::create(Mod::get()->getSavedValue<std::string>("image-theme-path").c_str()))
        {
            spr->setScaleX(panel->getContentWidth() / spr->getContentWidth());
            spr->setScaleY(panel->getContentHeight() / spr->getContentHeight());
            spr->setPosition(panel->getContentSize() / 2);
            spr->setZOrder(-2);

            panel->addChild(spr);
        }
    }

    return panel;
}

CCMenu* AndroidUI::getSearchPanel()
{
    auto menu = CCMenu::create();
    menu->setAnchorPoint(ccp(1, 0));
    menu->setPosition(ccp(475 - 15 + 5, 10));
    menu->setContentSize(ccp(340, panel->getContentSize().height - 10 - 10));
    menu->ignoreAnchorPointForPosition(false);
    menu->setID("search-results");

    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);

    menu->addChild(back);

    int y = 0;

    float gap = 28;
    float extraGap = 9.69f;
    float height = gap * roundUpToMultipleOf2(0 / 2);
    height = std::max<float>(menu->getContentHeight(), height + extraGap);
    
    scroll = geode::ScrollLayer::create(menu->getContentSize());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;
    menu->addChild(scroll);

    btnMenu = CCMenu::create();
    btnMenu->setContentSize(ccp(menu->getContentWidth(), height));
    btnMenu->setPosition(ccp(0, 0));
    btnMenu->setAnchorPoint(ccp(0, 0));
    scroll->m_contentLayer->addChild(btnMenu);

    scroll->m_contentLayer->setContentHeight(height);
    scroll->moveToTop();
    scroll->enableScrollWheel();

    menu->setVisible(false);
    return menu;
}

void AndroidUI::keyBackClicked()
{
    onClose(nullptr);
}

void AndroidUI::update(float dt)
{
    searchLabel->limitLabelWidth(90, 0.6f, 0.1f);
}

void AndroidUI::textChanged(CCTextInputNode* p0)
{
    for (size_t i = 0; i < pages.size(); i++)
    {
        pages[i]->setVisible(p0->getString().empty() ? (i == selectedTab) : false);
    }

    searchResultsPanel->setVisible(!p0->getString().empty());

    std::vector<Module*> modules = {};

    for (auto window : Client::instance->windows)
    {
        for (auto module : window->modules)
        {
            if (string::toLower(module->name).find(string::toLower(std::string(p0->getString()))) != std::string::npos)
            {
                if (!(module->id.starts_with("anim-speed")))
                    modules.push_back(module);
            }

            for (auto option : module->options)
            {
                if (string::toLower(option->name).find(string::toLower(std::string(p0->getString()))) != std::string::npos)
                {
                    if (!(option->id.starts_with("anim-speed")))
                        modules.push_back(option);
                }
            }
        }
    }

    int y = 0;

    float gap = 28;
    float extraGap = 9.69f;
    float height = gap * roundUpToMultipleOf2((modules.size() - 1) / 2);
    height = std::max<float>(scroll->getContentHeight(), height + extraGap);

    btnMenu->removeAllChildrenWithCleanup(true);
    btnMenu->setContentHeight(height);

    for (size_t m = 0; m < modules.size(); m++)
    {
        float x = 20;

        if (!(m % 2 == 0))
            x = 188;

        modules[m]->makeAndroid(btnMenu, {x, height - (gap * y) - (gap / 2) - (extraGap / 2)});

        if ((m - 1) % 2 == 0 && m != 0)
            y++;
    }

    scroll->m_contentLayer->setContentHeight(height);
    scroll->moveToTop();
    scroll->enableScrollWheel();
}

void AndroidUI::goToPage(int p, bool transition)
{
    selectedTab = p;

    if (transition)
    {
        
    }
    else
    {
        for (size_t i = 0; i < pages.size(); i++)
        {
            pages[i]->setVisible(i == p);
        }
        
    }
}

void AndroidUI::onClose(CCObject* sender)
{
    if (auto pause = getChildOfType<PauseLayer>(CCScene::get(), 0))
    {
        handleTouchPriority(pause);
    }

    CCTouchDispatcher::get()->unregisterForcePrio(this);
    CCTouchDispatcher::get()->removeDelegate(this);

    if (CCTouchDispatcher::get()->m_pTargetedHandlers->containsObject(this))
        CCTouchDispatcher::get()->m_pTargetedHandlers->removeObject(this, false);

    this->removeFromParent();
}

CCAction* AndroidUI::getEnterAction(CCNode* panel)
{
    float v = SpeedhackTop::getAdjustedValue();

    int e = Mod::get()->getSavedValue<int>("anim-mode", 2);

    panel->setPosition(CCDirector::get()->getWinSize() / 2);

    if (e == 1)
        panel->setPositionY(panel->getContentSize().height);
    if (e == 2)
        panel->setPositionY(-1 * panel->getContentSize().height / 2);
    if (e == 3)
        panel->setPositionX(-1 * panel->getContentSize().width / 2 / 2);
    if (e == 4)
        panel->setPositionX(panel->getContentSize().width);


    if (e == 5)
    {
        panel->setScale(0);

        #ifdef GEODE_IS_APPLE
        return CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f);
        #else
        return CCSpeed::create((CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f)), 1.0f / v);
        #endif
    }

    #ifdef GEODE_IS_APPLE
    return CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2), 0.3f);
    #else
    return CCSpeed::create((CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2))), 1.0f / v);
    #endif
}

void AndroidUI::onPressTab(CCObject* sender)
{
    auto btn = static_cast<CCMenuItemSprite*>(sender);

    if (typeinfo_cast<IconEffects*>(Client::instance->windows[btn->getTag()]) && EffectUI::getIncompatibleModLoaded())
        return FLAlertLayer::create(nullptr, "Icon Effects", fmt::format("Icon Effects have been disabled due to incompatibilities.\nTo use icon effects, disable the following mod:\n{}", EffectUI::getIncompatibleMods()), "OK", nullptr, 330, false, 300, 0.75f)->show();

    lastTab = selectedTab;
    selectedTab = btn->getTag();

    auto input = inputField;
    input->setString("");

    searchResultsPanel->setVisible(false);

    updateTabs();
    
    goToPage(selectedTab);
}

void AndroidUI::updateTabs()
{
    int i = 0;
    for (auto sprite : sprites)
    {
        sprite->updateSelection(i == selectedTab ? CategorySelectionType::Selected : CategorySelectionType::Deselected);
        as<CCMenuItemSpriteExtra*>(sprite->getParent())->setEnabled(i != selectedTab);
        i++;
    }
}

AndroidUI* AndroidUI::create()
{
    auto pRet = new AndroidUI();

    if (pRet->initAnchored(240.f, 160.f)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AndroidUI* AndroidUI::addToScene()
{
    if (auto existing = getChildOfType<AndroidUI>(CCScene::get(), 0))
        return existing;

    auto pRet = AndroidUI::create();

    CCScene::get()->addChild(pRet, 69420);
    return pRet;
}

AndroidUI::~AndroidUI()
{
    instance = nullptr;
}