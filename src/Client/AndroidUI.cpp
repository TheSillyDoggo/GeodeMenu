#include "AndroidUI.h"

bool AndroidUI::init()
{
    if (!CCLayerColor::init())
        return false;

    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setMouseEnabled(true);
    this->setKeypadEnabled(true);

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);

    this->runAction(CCFadeTo::create(0.5f, 100));
    this->setID("android-ui");

    auto backMenu = CCMenu::create();
    backMenu->ignoreAnchorPointForPosition(false);
    backMenu->setContentSize(ccp(0, 0));
    backMenu->setPositionX(0);
    backMenu->setPositionY(CCDirector::get()->getWinSize().height);

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    
    float v = 1.0f;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            auto x = numFromString<float>(SpeedhackTop::instance->text);

            if (x.isOk())
            {
                v = x.value();
                
                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;
            }
            else
                v = 1;

            bool m = SpeedhackMus::instance->enabled;

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            v /= CCDirector::get()->getScheduler()->getTimeScale();
        }
    }

    backSpr->runAction(CCSpeed::create(CCFadeIn::create(0.5f), 1.0f / v));

    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(AndroidUI::close));
    backBtn->setPosition(ccp(24, -23));
    backBtn->setSizeMult(1.15f);

    backMenu->addChild(backBtn);
    this->addChild(backMenu);

    
    int theme = Mod::get()->getSavedValue<int>("theme", 5);

    std::stringstream ss;
    ss << "GJ_square0";
    ss << (theme == -1 ? 6 : theme);
    ss << ".png";

    panel = CCScale9Sprite::create(ss.str().c_str());
    panel->setContentSize(ccp(475, 280));
    panel->setID("panel");

    as<CCNode*>(panel->getChildren()->objectAtIndex(0))->setZOrder(-2);

    if (Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages"))
    {
        auto size = panel->getContentSize();

        auto gradient = CCLayerGradient::create();
        gradient->setContentSize(size);
        gradient->setZOrder(-1);
        gradient->setID("gradient"_spr);

        if (Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages")->getSettingValue<bool>("use-custom-colours"))
        {
            gradient->setStartColor(Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages")->getSettingValue<ccColor3B>("primary-colour"));
            gradient->setEndColor(Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages")->getSettingValue<ccColor3B>("secondary-colour"));
        }
        else
        {
            gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
            gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
        }

        gradient->setPosition(CCDirector::get()->getWinSize() / 2);
        gradient->ignoreAnchorPointForPosition(false);

        if (Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages")->getSettingValue<bool>("reverse-order"))
            gradient->setScaleY(-1);

        auto outline = CCScale9Sprite::createWithSpriteFrameName((std::string("TheSillyDoggo.GradientPages/") + std::string("square-outline.png")).c_str());
        outline->setPosition(size / 2);
        outline->setContentSize(size);
        outline->setZOrder(1);
        outline->setID("outline"_spr);
        
        gradient->addChild(outline);

        panel->addChild(gradient);

        gradient->setAnchorPoint(ccp(0, 0));
        gradient->setPosition(ccp(0, 0));

        gradient->setVisible(theme == -1);
    }

    auto windows = CCScale9Sprite::create("square02_small.png");
    windows->setOpacity(100);
    windows->setPosition(ccp(10, 10));
    windows->setContentSize(ccp(110, panel->getContentSize().height - 10 - 10));
    windows->setAnchorPoint(ccp(0, 0));

    auto windowsMenu = CCMenu::create();
    windowsMenu->setContentSize(ccp(windows->getContentSize().width, windows->getContentSize().height - 10));
    windowsMenu->setAnchorPoint(ccp(0, 0));
    windowsMenu->setPosition(ccp(5, 5));
    windowsMenu->ignoreAnchorPointForPosition(false);

    windowsMenu->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(3.5f));
    windows->addChild(windowsMenu);

    labels.clear();
    buttons.clear();
    pages.clear();

    for (size_t i = 0; i < Client::instance->windows.size(); i++)
    {
        auto win = Client::instance->windows[i];

        auto btn = CCScale9Sprite::create("square02_small.png");
        btn->setContentSize(ccp(100, 20) / 0.5f);
        btn->setScale(0.5f);
        btn->setOpacity(100);

        auto lbl = CCLabelBMFont::create(win->name.c_str(), "bigFont.fnt");
        lbl->setPosition(btn->getContentSize() / 2);
        lbl->limitLabelWidth(100 / 0.5f, 0.75f, 0.1f);
        lbl->setColor({200, 200, 200});

        btn->addChild(lbl);

        auto btn2 = CCScale9Sprite::create("square02_small.png");
        btn2->setContentSize(ccp(100, 20) / 0.5f);
        btn2->setScale(0.5f);
        btn2->setOpacity(100);

        auto lbl2 = CCLabelBMFont::create(win->name.c_str(), "bigFont.fnt");
        lbl2->setPosition(btn->getContentSize() / 2);

        if (selectedTab == i)
        {
            lbl2->setColor({255, 255, 255});
            lbl2->setOpacity(255);
        }
        else
        {
            lbl2->setColor({150, 150, 150});
            lbl2->setOpacity(150);
        }

        lbl2->limitLabelWidth(100 / 0.5f, 0.75f, 0.1f);

        btn2->addChild(lbl2);
        
        auto b = CCMenuItemSprite::create(btn2, btn, windowsMenu, menu_selector(AndroidUI::onPressTab));
        b->setTag(i);
        b->setEnabled(i != selectedTab);
        b->setContentSize(b->getContentSize() / 2);

        windowsMenu->addChild(b);

        labels.push_back(lbl2);
        buttons.push_back(b);
    }

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

    windowsMenu->updateLayout();
    
    panel->addChild(windows);
    this->addChild(panel);

    std::stringstream ver;
    ver << "Using version " << Mod::get()->getVersion().getMajor() << "." << Mod::get()->getVersion().getMinor() << "." << Mod::get()->getVersion().getPatch();

    auto versionText = CCLabelBMFont::create(ver.str().c_str(), "chatFont.fnt");
    versionText->setColor({0, 0, 0});
    versionText->setOpacity(100);
    versionText->setAnchorPoint(ccp(0.5f, 0));
    versionText->setScale(0.45f);
    versionText->setPosition(ccp(64, 13 + 8));
    panel->addChild(versionText);

    auto devText = CCLabelBMFont::create("Mod Developed By TheSillyDoggo", "chatFont.fnt");
    devText->setColor({0, 0, 0});
    devText->setOpacity(100);
    devText->setAnchorPoint(ccp(0.5f, 0));
    devText->setScale(0.45f);
    devText->setPosition(ccp(64, 13));
    panel->addChild(devText);

    goToPage(selectedTab);

    panel->runAction(getEnterAction(panel));

    if (Client::GetModuleEnabled("npesta-width"))
    {
        panel->setPositionX(panel->getPositionX() + 5);
        panel->setContentSize(panel->getContentSize() + ccp(10, 0));
        as<CCNode*>(panel->getChildren()->objectAtIndex(0))->setPositionX(-5);
    }

    cocos::handleTouchPriority(this);

    return true;
}


void AndroidUI::keyBackClicked()
{
    close(nullptr);
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

void AndroidUI::close(CCObject* sender)
{
    if (auto pause = getChildOfType<PauseLayer>(CCScene::get(), 0))
    {
        handleTouchPriority(pause);
    }

    CCTouchDispatcher::get()->removeDelegate(this);
    this->removeFromParent();
}

CCAction* AndroidUI::getEnterAction(CCNode* panel)
{
    float v = 1.0f;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            auto x = numFromString<float>(SpeedhackTop::instance->text);

            if (x.isOk())
            {
                v = x.value();
                
                if (v < 0.01f)
                    v = 0.01f;

                if (v > 99999)
                    v = 99999;
            }
            else
                v = 1;

            bool m = SpeedhackMus::instance->enabled;

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            v /= CCDirector::get()->getScheduler()->getTimeScale();
        }
    }

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

        return CCSpeed::create((CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f)), 1.0f / v);
    }

    return CCSpeed::create((CCEaseElasticOut::create(CCMoveTo::create(1, CCDirector::get()->getWinSize() / 2))), 1.0f / v);
}

void AndroidUI::onPressTab(CCObject* sender)
{
    auto btn = static_cast<CCMenuItemSprite*>(sender);

    if (typeinfo_cast<_Replay*>(Client::instance->windows[btn->getTag()]))
        return FLAlertLayer::create("Replay", "Replay bot is not available right now as i'm trying to improve it <cp>:3</c>", "OK")->show();

    #ifdef GEODE_IS_MACOS
    if (typeinfo_cast<_Replay*>(Client::instance->windows[btn->getTag()]))
        return FLAlertLayer::create("Replay", "Replay bot is not available on macos due to missing bindings, this will probably <cr>not</c> be fixed <cp>^w^</c>", "OK")->show();
    #endif

    lastTab = selectedTab;
    selectedTab = btn->getTag();

    for (size_t i = 0; i < labels.size(); i++)
    {
        buttons[i]->setEnabled(i != selectedTab);

        if (selectedTab == i)
        {
            labels[i]->setColor({255, 255, 255});
            labels[i]->setOpacity(255);
        }
        else
        {
            labels[i]->setColor({150, 150, 150});
            labels[i]->setOpacity(150);
        }

        labels[i]->updateLabel();
    }
    
    log::info("Changed tab to {}", selectedTab);
    goToPage(selectedTab);
}