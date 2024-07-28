#include "Labels.hpp"

/*
replay->modules.push_back(new StatusOpacity());
replay->modules.push_back(new StatusScale());

//replay->modules.push_back(new Module("Testmode", "status-testmode", "Show the test mode text if there's a startpos"));
replay->modules.push_back(new Module("Cheat Indicator", "cheat-indicator", "Shows if you are cheating"));
replay->modules.push_back(new Module("FPS Counter", "status-fps", "Shows your current game fps"));
replay->modules.push_back(new Module("Noclip Accuracy", "status-accuracy", "Shows your accuracy in noclip (hidden when noclip is disabled)"));
replay->modules.push_back(new Module("Noclip Deaths", "status-deaths", "Shows your deaths in noclip (hidden when noclip is disabled)"));
replay->modules.push_back(new Module("Attempts", "status-attempt", "Shows Attempt Count"));
//replay->modules.push_back(new Module("Clicks", "status-cps", "Shows Attempt Count"));

replay->modules.push_back(new Module("Replay Status", "status-replay", "Replay debug info"));
//replay->modules.push_back(new Module("Noclip Deaths (not fully accurate)", "status-death", "Shows your death count (hidden when noclip is disabled)"));
//replay->modules.push_back(new Module("Noclip Accuracy", "status-accuracy", "Shows your death accuracy (hidden when noclip is disabled)"));
//replay->modules.push_back(new Module("Attempts", "status-attempts", "Shows your attempt count"));
//replay->modules.push_back(new Module("Clicks", "status-clicks", "Shows your click count"));
replay->modules.push_back(new Module("Message", "status-message", "Write a message of your choice to be shown"));
replay->modules.push_back(new Module("Session Time", "status-session", "Shows the time you've had the game open for in the format <cg>hh::mm::ss</c>"));
replay->modules.push_back(new Module("Clicks Per Second", "status-cps", "Shows your clicks per second. Tints <cg>Green</c> while you are clicking"));
//replay->modules.push_back(new StatusMessage());



for (auto mod : replay->modules)
{
    mod->options.push_back(new DropdownModule({"Top Left", "Top Right", "Bottom Left", "Bottom Right"}, mod->id + "-side", 0));
}

auto messageOption = new InputModule("Message Text: ", "status-message-text", "Default Message");
messageOption->allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{};:\'\",.<>/?|`~ ";
messageOption->maxSize = 48; // its just a bit before it overflows on 16:9, perfect
Client::GetModule("status-message")->options.push_back(messageOption);
*/

void Labels::cocosCreate(CCMenu* menu)
{
    //return Window::cocosCreate(menu); // todo: finish ui

    labels.clear();

    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);
    menu->addChild(back, -69);

    auto hideToggle = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Labels::onHide), 0.8f);
    hideToggle->toggle(Mod::get()->getSavedValue<bool>("hide-labels"));
    hideToggle->setPosition(ccp(0, menu->getContentHeight()) + ccp(18, -18));
    hideToggle->setUserData(this);
    menu->addChild(hideToggle);

    auto hideLbl = CCLabelBMFont::create("Hide Labels", "bigFont.fnt");
    hideLbl->setScale(0.5f);
    hideLbl->setPosition(hideToggle->getPosition() + ccp(17, 0));
    hideLbl->setAnchorPoint(ccp(0, 0.5f));
    menu->addChild(hideLbl, -1);

    auto scroll = ScrollLayer::create(ccp(menu->getContentWidth() - 20, menu->getContentHeight() - 40 - 10 + 2));
    scroll->setPosition(ccp(10, 10));
    scroll->m_contentLayer->setContentHeight(30 * (modules.size() - 2));
    scroll->moveToTop();
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;
    menu->addChild(scroll);

    auto bg = CCLayerColor::create(ccc4(0, 0, 0, 0));
    bg->setOpacity(75);
    bg->setContentSize(scroll->getContentSize() + ccp(4, 4));
    bg->setPosition(scroll->getPosition() + ccp(-2, -2));
    menu->addChild(bg, -3);

    for (size_t i = 2; i < modules.size(); i++)
    {
        auto mod = modules[i];

        auto cell = CCMenu::create();
        cell->setContentSize(ccp(scroll->getContentWidth(), 30));
        cell->setPosition(ccp(0, ((modules.size() - i) - 1) * 30));
        cell->setAnchorPoint(ccp(0, 0));

        auto bg = CCLayerColor::create(ccc4(0, 0, 0, 0));
        bg->setOpacity((i & 1) ? 100 : 35);
        bg->setContentSize(cell->getContentSize());
        cell->addChild(bg);

        auto toggle = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Labels::onToggleLabel), 0.65f);
        toggle->toggle(mod->enabled);
        toggle->setUserData(mod);
        toggle->setPosition(ccp(30 / 2, 30 / 2));
        cell->addChild(toggle);

        auto name = CCLabelBMFont::create(mod->name.c_str(), "bigFont.fnt");
        name->setAnchorPoint(ccp(0, 0.5f));
        name->setPosition(toggle->getPosition() + ccp(15, 0));
        name->setScale(0.6f);

        if (hideToggle->isToggled())
            name->setOpacity(150);

        cell->addChild(name);

        if (!mod->options.empty())
        {
            auto spr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
            spr->setScale(0.55f);

            auto btn = CCMenuItemSpriteExtra::create(spr, menu, menu_selector(Labels::onLabelOptions));
            btn->setUserData(mod);
            btn->setPosition(ccp(cell->getContentWidth() - 15, 30 / 2));

            cell->addChild(btn);
        }

        labels.push_back(name);
        scroll->m_contentLayer->addChild(cell);
    }

    error = TextArea::create("Nothing here yet <cl>:(</c>", "bigFont.fnt", 1, 1000, ccp(0.5f, 0.5f), 10, false);
    error->setVisible(modules.size() == 0);
    error->setPosition(bg->getPosition() + bg->getContentSize() / 2);
    error->setScale(0.65f);
    menu->addChild(error);
}

void Labels::onHide(CCObject* sender)
{
    bool v = !Mod::get()->setSavedValue("hide-labels", !Mod::get()->getSavedValue<bool>("hide-labels"));

    for (auto name : as<Labels*>(as<CCNode*>(sender)->getUserData())->labels)
    {
        name->setOpacity(v ? 150 : 255);
    }

    #ifdef STATUS_TEXTS
    StatusNode::hidden = Mod::get()->getSavedValue<bool>("hide-labels");
    #endif
}

void Labels::onToggleLabel(CCObject* sender)
{
    auto mod = as<Module*>(as<CCNode*>(sender)->getUserData());

    mod->enabled = !mod->enabled;

    if (mod->delegate)
        mod->delegate->onModuleChanged(mod->enabled);
}

void Labels::onLabelOptions(CCObject* sender)
{
    auto mod = as<Module*>(as<CCNode*>(sender)->getUserData());

    mod->onOptionsAndroid(sender);
}

void Labels::loadConfig()
{
    if (Mod::get()->hasSavedValue("labels-config"))
    {
        auto cfg = Mod::get()->getSavedValue<matjson::Array>("labels-config");

        for (auto child : cfg)
        {
            auto obj = child.as_object();
            
            auto mod = new LabelModule("", "");
            mod->name = obj["name"].as_string();
            mod->format = obj["format"].as_string();
            mod->setFont(obj["font"].as_string());
            mod->setSide(as<LabelSide>(obj["side"].as_int()));
            mod->setOpacity(obj["opacity"].as_double());
            mod->setScale(obj["scale"].as_double());

            modules.push_back(mod);
        }
    }
}

void Labels::saveConfig()
{

}
