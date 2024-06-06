#include "Config.h"
#include "../Client/AndroidBall.h"

void Config::cocosCreate(CCMenu* menu)
{
    tabs.clear();
    btns.clear();
    btnsS.clear();
    this->menu = menu;

    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);
    menu->addChild(back);

    if (!Mod::get()->getSavedValue<bool>("hasShownDCMsg", false))
    {
        sprJoin = CCSprite::create("joinDiscord.png"_spr);
        sprJoin->setPosition(menu->getContentSize() + ccp(-50, -40));

        menu->addChild(sprJoin, 42069);
    }

    auto tabsMenu = CCMenu::create();
    tabsMenu->setContentWidth(6969);
    tabsMenu->setPositionX(menu->getContentWidth() / 2);
    tabsMenu->setPositionY(menu->getContentHeight() - 15);
    tabsMenu->setScale(0.5f);
    tabsMenu->setLayout(RowLayout::create()->setGap(12.5f)->setAutoScale(false));

    tabsMenu->addChild(createTabButton("Menu", 1));
    tabsMenu->addChild(createTabButton("Button", 2));
    tabsMenu->addChild(createTabButton("Theme", 3));
    tabsMenu->addChild(createTabButton("About", 4));

    tabsMenu->updateLayout();
    menu->addChild(tabsMenu);

    auto menuTab = CCMenu::create();
    menuTab->setContentWidth(menu->getContentWidth());
    menuTab->setContentHeight(menu->getContentHeight() - 32);
    menuTab->setPosition(ccp(0, 0));
    menuTab->setAnchorPoint(ccp(0, 0));
    tabs.push_back(menuTab);
    menu->addChild(menuTab);

    auto animTitle = CCLabelBMFont::create("Animation: ", "bigFont.fnt");
    animTitle->setAnchorPoint(ccp(0, 1));
    animTitle->setScale(0.55f);
    menuTab->addChildAtPosition(animTitle, Anchor::TopLeft, ccp(5, -4));

    animDropdown = Dropdown::create({130, 25}, {"None", "From Top", "From Bottom", "From Left", "From Right", "Scale"}, menu_selector(Config::onDropdownChanged), Mod::get()->getSavedValue<int>("anim-mode", 2));
    animDropdown->setZOrder(42069);
    menuTab->addChildAtPosition(animDropdown, Anchor::TopLeft, ccp(5 + animTitle->getScaledContentSize().width + 2, -2 - 25));

    #ifdef GEODE_IS_DESKTOP
    for (size_t i = 1; i < 4; i++)
    #else
    for (size_t i = 1; i < 3; i++)
    #endif
    {
        modules[i]->makeAndroid(menuTab, ccp(20, menuTab->getContentHeight() - 45 - (28 * (i - 1))));
    }
    
    auto buttonTab = CCMenu::create();
    buttonTab->setContentWidth(menu->getContentWidth());
    buttonTab->setContentHeight(menu->getContentHeight() - 32);
    buttonTab->setPosition(ccp(0, 0));
    buttonTab->setAnchorPoint(ccp(0, 0));
    tabs.push_back(buttonTab);
    menu->addChild(buttonTab);

    float posY = 0;
    posY += 17;

    Client::GetModule("save-pos")->makeAndroid(buttonTab, ccp(17, posY) - buttonTab->getContentSize());
    posY += 30;

    Client::GetModule("allow-dragging")->makeAndroid(buttonTab, ccp(17, posY) - buttonTab->getContentSize());
    posY += 10;

    auto lSc = CCLabelBMFont::create("Scale:", "bigFont.fnt");
    lSc->setPosition(ccp(5, posY + 23));
    posY += 27;
    lSc->setAnchorPoint(ccp(0, 1));
    lSc->setScale(0.5f);

    scale = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    scale->setPosition(ccp(lSc->getPositionX() + 85, lSc->getPositionY() - 10));
    scale->setScaleX(0.8f);
    scale->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);
    scale->setValue((Mod::get()->getSavedValue<float>("button-scale", 1)));

    auto floor = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    posY += 2;
    floor->setScaleX(0.45f);
    floor->setOpacity(100);
    floor->setPosition(ccp(105, posY));

    auto misc = CCLabelBMFont::create("Misc", "bigFont.fnt");
    posY += 12;
    misc->setScale(0.65f);
    misc->setPosition(ccp(105, posY));

    posY += 25;

    Client::GetModule("instant-fade")->makeAndroid(buttonTab, ccp(17, posY) - buttonTab->getContentSize());

    posY += 13;

    posY += 22 * 3;

    auto lNormal = CCLabelBMFont::create("Normal:", "bigFont.fnt");
    lNormal->setPosition(ccp(5, posY));
    lNormal->setAnchorPoint(ccp(0, 1));
    lNormal->setScale(0.5f);

    normal = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    normal->setValue((Mod::get()->getSavedValue<int>("normal-opacity", 255) / 255.0f));
    normal->setPosition(ccp(lNormal->getPositionX() + 85, lNormal->getPositionY() - 10));
    normal->setScaleX(0.8f);
    normal->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

    posY -= 22;

    auto lGP = CCLabelBMFont::create("Gameplay:", "bigFont.fnt");
    lGP->setPosition(ccp(5, posY));
    lGP->setAnchorPoint(ccp(0, 1));
    lGP->setScale(0.5f);

    GP = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    GP->setValue((Mod::get()->getSavedValue<int>("gameplay-opacity", 50) / 255.0f));
    GP->setPosition(ccp(lGP->getPositionX() + 85, lGP->getPositionY() - 10));
    GP->setScaleX(0.8f);
    GP->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

    posY -= 22;

    auto lED = CCLabelBMFont::create("Editor:", "bigFont.fnt");
    lED->setPosition(ccp(5, posY));
    lED->setAnchorPoint(ccp(0, 1));
    lED->setScale(0.5f);

    ED = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    ED->setValue((Mod::get()->getSavedValue<int>("editor-opacity", 50) / 255.0f));
    ED->setPosition(ccp(lED->getPositionX() + 85, lED->getPositionY() - 10));
    ED->setScaleX(0.8f);
    ED->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);


    posY += 22 * 2;

    auto floor2 = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    posY += 2;
    floor2->setScaleX(0.45f);
    floor2->setOpacity(100);
    floor2->setPosition(ccp(105, posY));

    auto opac = CCLabelBMFont::create("Opacity", "bigFont.fnt");
    posY += 12;
    opac->setScale(0.65f);
    opac->setPosition(ccp(105, posY));


    auto previewBG = CCLayerColor::create(ccc4(0, 0, 0, 255));
    previewBG->setOpacity(255);
    previewBG->setContentSize(ccp(120, 120));
    previewBG->setAnchorPoint(ccp(1, 0));
    previewBG->ignoreAnchorPointForPosition(false);

    auto previewTitle = CCLabelBMFont::create("Preview", "bigFont.fnt");
    previewTitle->setScale(0.5f);

    btnMenu = CCMenu::create();
    btnMenu->setScale(AndroidBall::clampf(Mod::get()->getSavedValue<float>("button-scale", 1), 0.2f, 1));
    btnMenu->setContentSize(ccp(0, 0));

    btnL = CCPastelLabelBMFont::create(">_", "bigFont.fnt");
    btnL->setAnchorPoint(ccp(0.5f, 0.35f));

    btn = CircleButtonSprite::create(btnL, CircleBaseColor::Gray);
    btnMenu->addChild(btn);

    btn->setOpacity(Mod::get()->getSavedValue<int>("normal-opacity", 255));
    btnL->setOpacity(Mod::get()->getSavedValue<int>("normal-opacity", 255));

    buttonTab->addChildAtPosition(previewBG, Anchor::BottomRight, ccp(-10, 10));
    previewBG->addChildAtPosition(previewTitle, Anchor::Top, ccp(0, -11));
    previewBG->addChildAtPosition(btnMenu, Anchor::Center);

    auto disableGP = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Config::onDisableGP), 0.69420f);
    disableGP->toggle(Mod::get()->getSavedValue<bool>("disable-gp_enabled", true));

    auto disableLabel = CCLabelBMFont::create("Disable ingame:", "bigFont.fnt");
    disableLabel->setScale(0.4f);
    disableLabel->setAnchorPoint(ccp(0, 0));

    buttonTab->addChildAtPosition(disableGP, Anchor::BottomRight, ccp(-15, 145));
    buttonTab->addChildAtPosition(disableLabel, Anchor::BottomRight, ccp(-135, 140));
    buttonTab->addChild(floor);
    buttonTab->addChild(misc);
    buttonTab->addChild(floor2);
    buttonTab->addChild(opac);
    buttonTab->addChild(lNormal);
    buttonTab->addChild(normal);
    buttonTab->addChild(lGP);
    buttonTab->addChild(GP);
    buttonTab->addChild(lED);
    buttonTab->addChild(ED);
    buttonTab->addChild(lSc);
    buttonTab->addChild(scale);

    auto themeTab = CCMenu::create();
    themeTab->setContentWidth(menu->getContentWidth());
    themeTab->setContentHeight(menu->getContentHeight() - 32);
    themeTab->setPosition(ccp(0, 0));
    themeTab->setAnchorPoint(ccp(0, 0));
    tabs.push_back(themeTab);
    menu->addChild(themeTab);

    auto m = CCMenu::create();
    m->setAnchorPoint(back->getAnchorPoint());
    m->setScale(0.45f);
    m->setPosition(back->getPosition() + ccp(10, -10));
    m->setContentSize(back->getContentSize());

    createBtn(m, 1);
    createBtn(m, 2);
    createBtn(m, 3);
    createBtn(m, 4);
    createBtn(m, 5);
    createBtn(m, -1);
    createBtn(m, -2);
    //createBtn(m, -3);

    //m->addChild(CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_epicCoin3_001.png"), menu, menu_selector(Config::onChangeFile)));

    m->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(10));
    m->updateLayout();
    themeTab->addChild(m);

    auto aboutTab = CCMenu::create();
    aboutTab->setContentWidth(menu->getContentWidth());
    aboutTab->setContentHeight(menu->getContentHeight() - 32);
    aboutTab->setPosition(ccp(0, 0));
    aboutTab->setAnchorPoint(ccp(0, 0));
    tabs.push_back(aboutTab);
    menu->addChild(aboutTab);

    auto discord = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"), menu, menu_selector(Config::onLink)); // https://discord.gg/DfQSTEnQKK
    discord->setPosition(ccp(menu->getContentSize().width, 0) + ccp(-10, 12));
    discord->setID("https://discord.gg/DfQSTEnQKK");
    aboutTab->addChild(discord);

    auto yt = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png"), menu, menu_selector(Config::onLink)); // https://www.youtube.com/@TheSillyDoggo
    yt->setPosition(ccp(menu->getContentSize().width, 0) + ccp(-10, 12) + ccp(0, 35));
    yt->setID("https://www.youtube.com/@TheSillyDoggo");
    aboutTab->addChild(yt);

    for (size_t i = 0; i < tabs.size(); i++)
    {
        tabs[i]->setVisible(i == selectedTab - 1);
    }
}

CCMenuItemToggler* Config::createTabButton(std::string name, int index)
{
    auto btn = CCMenuItemToggler::create(ButtonSprite::create(name.c_str(), "bigFont.fnt", "GJ_button_04.png"), ButtonSprite::create(name.c_str(), "bigFont.fnt", "GJ_button_05.png"), this->menu, menu_selector(Config::onChangeTab));
    btn->setTag(index);

    if (index == selectedTab)
    {
        btn->toggle(true);
        btn->setEnabled(false);
    }

    return btn;
}

void Config::onChangeTab(CCObject* sender)
{
    this->selectedTab = as<CCMenuItemToggler*>(sender)->getTag();

    if (selectedTab == 4)
    {
        Mod::get()->setSavedValue<bool>("hasShownDCMsg", true);

        if (sprJoin)
            sprJoin->setVisible(false);
    }
    
    CCArrayExt<CCMenuItemToggler*> tabs = as<CCNode*>(sender)->getParent()->getChildren();

    for (auto tab : tabs)
    {
        tab->setEnabled(tab->getTag() != selectedTab);
        tab->toggle(tab->getTag() == selectedTab);

        tab->m_onButton->setScale(1);
        tab->m_offButton->setScale(1);
    }

    for (size_t i = 0; i < Config::tabs.size(); i++)
    {
        Config::tabs[i]->setVisible(i == selectedTab - 1);
    }
}

void Config::changeTheme(CCObject* sender)
{
    int v = reinterpret_cast<CCNode*>(sender)->getTag();

    if (v == -1)
    {
        if (!Loader::get()->getInstalledMod("TheSillyDoggo.GradientPages"))
        {
            return FLAlertLayer::create("Gradient Pages", "The <cg>Gradient</c> Theme option requires the mod, <cl>Gradient Pages</c> to be installed. Install <cl>Gradient Pages</c> to use gradient mode.", "OK")->show();
        }
        else if (!Loader::get()->getLoadedMod("TheSillyDoggo.GradientPages"))
        {
            return FLAlertLayer::create("Gradient Pages", "The <cg>Gradient</c> Theme option requires the mod, <cl>Gradient Pages</c> to be enabled. Enable <cl>Gradient Pages</c> to use gradient mode.", "OK")->show();
        }
    }

    Mod::get()->setSavedValue<int>("theme", v);

    log::info("change theme to {}", v);

    for (size_t i = 0; i < btns.size(); i++)
    {
        if (btnsS[i]->getTag() - 1 == v - 1)
        {
            btns[i]->setColor({255, 255, 255});
            btns[i]->setOpacity(255);
        }
        else
        {
            btns[i]->setColor({150, 150, 150});
            btns[i]->setOpacity(200);
        }

        btnsS[i]->setEnabled(i != v - 1);
    }
}

void Config::createBtn(CCNode* menu, int i)
{
    std::stringstream ss;
    ss << "GJ_square0";
    ss << (i < 0 ? 6 : i);
    ss << ".png";

    auto spr = CCScale9Sprite::create(ss.str().c_str());
    spr->setColor({150, 150, 150});
    spr->setOpacity(200);
    auto sprSel = CCScale9Sprite::create(ss.str().c_str());
    sprSel->setColor({200, 200, 200});

    spr->setContentSize(ccp(100, 35) * 2);
    spr->setPosition(ccp(0, 0));
    sprSel->setContentSize(ccp(100, 35) * 2);
    sprSel->setPosition(ccp(0, 0));

    auto btn = CCMenuItemSpriteExtra::create(spr, sprSel, menu, menu_selector(Config::changeTheme));
    btn->setSelectedImage(sprSel);
    btn->setTag(i);
    btn->setContentSize(ccp(100, 35) * 2);
    btn->m_scaleMultiplier = 1.0f;

    btn->setEnabled(i != Mod::get()->getSavedValue<int>("theme", 5));

    if (!btn->isEnabled())
    {
        spr->setColor({255, 255, 255});
        spr->setOpacity(255);
    }

    if (i == -1)
    {
        auto gr = CCLabelBMFont::create("Gradient", "bigFont.fnt");
        gr->setOpacity(100);
        gr->setPosition(spr->getContentSize() / 2);

        spr->addChild(gr);
        sprSel->addChild(gr);
    }
    
    if (i == -2)
    {
        auto gr = CCLabelBMFont::create("Darken", "bigFont.fnt");
        gr->setOpacity(100);
        gr->setPosition(spr->getContentSize() / 2);

        spr->addChild(gr);
        sprSel->addChild(gr);
    }

    if (i == -3)
    {
        auto gr = CCLabelBMFont::create("Custom Image", "bigFont.fnt");
        gr->setOpacity(100);
        gr->setPosition(spr->getContentSize() / 2);

        spr->addChild(gr);
        sprSel->addChild(gr);
    }

    menu->addChild(btn);

    btns.push_back(spr);
    btnsS.push_back(btn);
}

void Config::onDropdownChanged(CCObject*)
{
    Mod::get()->setSavedValue<int>("anim-mode", animDropdown->getSelectedIndex());
}

void Config::onLink(CCObject* sender)
{
    auto a = geode::createQuickPopup(
        "Hold Up!",
        "Links are spooky! Are you sure you want to go to\n<cy>" + std::string(as<CCNode*>(sender)->getID()) + "</c>?",
        "Cancel", "Yes",
        [](FLAlertLayer* a, bool btn2) {
            if (btn2) {
                CCApplication::get()->openURL(a->getID().c_str());
            }
        }
    );

    a->setID(as<CCNode*>(sender)->getID());
}

void Config::onSliderChanged(CCObject* sender)
{
    float ov = (normal->getThumb()->getPositionX() + 50.0f) / 100.0f;
    int v = round(ov * 255.0f);
    if (v < 10)
        v = 10;

    Mod::get()->setSavedValue<int>("normal-opacity", (int)(v));

    float ovgp = (GP->getThumb()->getPositionX() + 50.0f) / 100.0f;
    int vgp = round(ovgp * 255.0f);

    Mod::get()->setSavedValue<int>("gameplay-opacity", (int)(vgp));

    float oved = (ED->getThumb()->getPositionX() + 50.0f) / 100.0f;
    int ved = round(oved * 255.0f);

    Mod::get()->setSavedValue<int>("editor-opacity", (int)(ved));

    Mod::get()->setSavedValue<float>("button-scale", scale->getThumb()->getValue());

    btnMenu->setScale(AndroidBall::clampf(Mod::get()->getSavedValue<float>("button-scale", 1), 0.2f, 1));

    btn->stopAllActions();
    btnL->stopAllActions();
    
    btn->runAction(CCFadeTo::create(Client::GetModuleEnabled("instant-fade") ? 0 : 0.35f, Mod::get()->getSavedValue<int>("normal-opacity", 255)));
    btnL->runAction(CCFadeTo::create(Client::GetModuleEnabled("instant-fade") ? 0 : 0.35f, Mod::get()->getSavedValue<int>("normal-opacity", 255)));

    //"instant-fade"
}

void Config::onDisableGP(CCObject* sender)
{
    Mod::get()->setSavedValue<bool>("disable-gp_enabled", !Mod::get()->getSavedValue<bool>("disable-gp_enabled", true));
}

void Config::onChangeFile(CCObject*)
{
    /*
    file::FilePickOptions options = {
        std::nullopt,
        {}
    };

    auto callback = [this](std::filesystem::path result) {
        auto path = std::filesystem::path(result.c_str());
        #ifdef GEODE_IS_WINDOWS
        auto strPath = geode::utils::string::wideToUtf8(result.c_str());
        #else
        std::string strPath = result.c_str();
        #endif
        log::info("image path: {}", strPath);

        Mod::get()->setSavedValue<std::string>("image-theme-path", strPath);
    };
    auto failedCallback = []() {
        FLAlertLayer::create("Error", "Failed to open file", "Ok")->show();
    };

    file::pickFile(file::PickMode::OpenFile, options, callback, failedCallback);*/
}