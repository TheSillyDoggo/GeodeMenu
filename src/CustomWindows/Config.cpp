#include "Config.h"

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


    for (size_t i = 1; i < 2; i++)
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

    for (size_t i = 2; i < modules.size(); i++)
    {
        modules[i]->makeAndroid(buttonTab, ccp(20, buttonTab->getContentHeight() - 15 - (28 * (i - 2))));
    }

    auto lNormal = CCLabelBMFont::create("Normal:", "bigFont.fnt");
    lNormal->setPosition(ccp(5, 115 - 5));
    lNormal->setAnchorPoint(ccp(0, 1));
    lNormal->setScale(0.5f);

    normal = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    normal->setValue((Mod::get()->getSavedValue<int>("normal-opacity", 255) / 255.0f));
    normal->setPosition(ccp(lNormal->getPositionX() + 85, lNormal->getPositionY() - 10));
    normal->setScaleX(0.8f);
    normal->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

    auto lGP = CCLabelBMFont::create("Gameplay:", "bigFont.fnt");
    lGP->setPosition(ccp(5, 115 - 5 - 20));
    lGP->setAnchorPoint(ccp(0, 1));
    lGP->setScale(0.5f);

    GP = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    GP->setValue((Mod::get()->getSavedValue<int>("gameplay-opacity", 50) / 255.0f));
    GP->setPosition(ccp(lGP->getPositionX() + 85, lGP->getPositionY() - 10));
    GP->setScaleX(0.8f);
    GP->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

    auto lED = CCLabelBMFont::create("Editor:", "bigFont.fnt");
    lED->setPosition(ccp(5, 115 - 5 - 20 - 20));
    lED->setAnchorPoint(ccp(0, 1));
    lED->setScale(0.5f);

    ED = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    ED->setValue((Mod::get()->getSavedValue<int>("editor-opacity", 50) / 255.0f));
    ED->setPosition(ccp(lED->getPositionX() + 85, lED->getPositionY() - 10));
    ED->setScaleX(0.8f);
    ED->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

    auto lSc = CCLabelBMFont::create("Scale:", "bigFont.fnt");
    lSc->setPosition(ccp(5, 115 - 5 - 20 - 20 - 20));
    lSc->setAnchorPoint(ccp(0, 1));
    lSc->setScale(0.5f);

    scale = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
    scale->setValue((Mod::get()->getSavedValue<float>("button-scale", 1)));
    scale->setPosition(ccp(lED->getPositionX() + 85, lSc->getPositionY() - 10));
    scale->setScaleX(0.8f);
    scale->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

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

    m->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(10));
    m->updateLayout();
    themeTab->addChild(m);


    for (size_t i = 0; i < tabs.size(); i++)
    {
        tabs[i]->setVisible(i == selectedTab - 1);
    }

    return;

    menu->addChild(back);

    auto devText = CCLabelBMFont::create("Re-open menu to apply changes", "chatFont.fnt");
    devText->setColor({0, 0, 0});
    devText->setOpacity(100);
    devText->setAnchorPoint(ccp(0.5f, 0));
    devText->setScale(0.45f);
    devText->setPosition(ccp((menu->getContentSize().width / 2) + 65, 1));
    menu->addChild(devText);

    modules[0]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 28 - 20));
    modules[1]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 30 - 28 - 20));
    modules[2]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 30 - 28 - 28 - 20));
    modules[3]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 30 - 28 - 28 - 20 - 20));
    #ifdef GEODE_IS_DESKTOP
    modules[4]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 30 - 28 - 28 - 28 - 20 - 20));
    #endif

    menu->addChild(lNormal);
    menu->addChild(normal);
    menu->addChild(lGP);
    menu->addChild(GP);
    menu->addChild(lED);
    menu->addChild(ED);
    menu->addChild(lSc);
    menu->addChild(scale);

    auto discord = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"), menu, menu_selector(Config::onLink)); // https://discord.gg/DfQSTEnQKK
    discord->setPosition(ccp(menu->getContentSize().width, 0) + ccp(-10, 12));
    discord->setID("https://discord.gg/DfQSTEnQKK");
    menu->addChild(discord);

    auto yt = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png"), menu, menu_selector(Config::onLink)); // https://www.youtube.com/@TheSillyDoggo
    yt->setPosition(ccp(menu->getContentSize().width, 0) + ccp(-10, 12) + ccp(0, 35));
    yt->setID("https://www.youtube.com/@TheSillyDoggo");
    menu->addChild(yt);
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
    ss << (i == -1 ? 6 : i);
    ss << ".png";

    auto spr = CCScale9Sprite::create(ss.str().c_str());
    spr->setColor({150, 150, 150});
    spr->setOpacity(200);
    auto sprSel = CCScale9Sprite::create(ss.str().c_str());
    sprSel->setColor({200, 200, 200});

    auto btn = CCMenuItemSprite::create(spr, sprSel, menu, menu_selector(Config::changeTheme));
    btn->setTag(i);
    btn->setContentSize(ccp(100, 35) * 2);
    spr->setContentSize(ccp(100, 35) * 2);
    spr->setPosition(ccp(0, 0));
    sprSel->setContentSize(ccp(100, 35) * 2);
    sprSel->setPosition(ccp(0, 0));

    btn->setTag(i);

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
}