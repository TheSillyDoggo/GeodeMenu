#include "Labels.h"

void Labels::cocosCreate(CCMenu* menu)
{
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

    auto bg = CCLayerColor::create();
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

        auto bg = CCLayerColor::create();
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
}

void Labels::onHide(CCObject* sender)
{
    bool v = !Mod::get()->setSavedValue("hide-labels", !Mod::get()->getSavedValue<bool>("hide-labels"));

    for (auto name : as<Labels*>(as<CCNode*>(sender)->getUserData())->labels)
    {
        name->setOpacity(v ? 150 : 255);
    }

    StatusNode::hidden = Mod::get()->getSavedValue<bool>("hide-labels");
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