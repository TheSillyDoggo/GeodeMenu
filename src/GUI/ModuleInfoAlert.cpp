#include "ModuleInfoAlert.hpp"
#include <Module.hpp>
#include <LocalisationManager.hpp>
#include <Button.hpp>
#include <Categories/FavouritesNode.hpp>
#include <SetupShortcutUI.hpp>
#include <EditKeyConfigUI.hpp>

using namespace geode::prelude;
using namespace qolmod;

ModuleInfoAlert* ModuleInfoAlert::create(Module* module)
{
    auto pRet = new ModuleInfoAlert();
    
    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool ModuleInfoAlert::init(Module* module)
{
    auto ok = LocalisationManager::get()->getLocalisedString("ui/ok-button");

    mod = module;
    title = module->getName().c_str();
    desc = module->getDescription();
    width = 300.0f;
    btn1 = ok.c_str();
    btn2 = nullptr;
    BetterAlertLayer::init(0, 0);

    auto menu = CCMenu::create();
    menu->setPosition(ccp(0, 25));

    auto btn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), this, menu_selector(ModuleInfoAlert::onToggleFavourite));
    btn->setPositionX(25);
    btn->toggle(module->isFavourited());

    btn->setContentSize(btn->getContentSize() * 3);

    btn->m_offButton->setContentSize(btn->getContentSize());
    btn->m_offButton->setPosition(btn->getContentSize() / 2);
    btn->m_offButton->getNormalImage()->setPosition(btn->getContentSize() / 2);

    btn->m_onButton->setContentSize(btn->getContentSize());
    btn->m_onButton->setPosition(btn->getContentSize() / 2);
    btn->m_onButton->getNormalImage()->setPosition(btn->getContentSize() / 2);

    btn->m_offButton->setColor(ccc3(150, 150, 150));
    btn->m_offButton->setOpacity(150);

    auto btnKeybind = Button::create(CCSprite::create("keybinds.png"_spr), this, menu_selector(ModuleInfoAlert::onChangeKeybind));
    btnKeybind->setContentSize(btnKeybind->getContentSize() * ccp(1, 2));
    btnKeybind->setPositionX(m_mainLayer->getContentWidth() - 25);
    btnKeybind->getNormalImage()->setPosition(btnKeybind->getContentSize() / 2);

    auto btnShortcut = CCMenuItemToggler::create(CCSprite::create("shortcuts.png"_spr), CCSprite::create("shortcuts.png"_spr), this, menu_selector(ModuleInfoAlert::onChangeShortcut));
    btnShortcut->setPositionX(m_mainLayer->getContentWidth() - 25 - 25);

    menu->addChild(btn);
    menu->addChild(btnKeybind);
    menu->addChild(btnShortcut);
    m_mainLayer->addChild(menu, 8008569);
    return true;
}

void ModuleInfoAlert::onToggleFavourite(CCObject* sender)
{
    mod->setFavourited(!mod->isFavourited());

    if (FavouritesNode::get())
        FavouritesNode::get()->refresh();
}

void ModuleInfoAlert::onChangeKeybind(CCObject* sender)
{
    auto ui = EditKeyConfigUI::create([this](KeyConfigStruct config)
    {
        mod->setKeybind(config);
    });

    ui->setDefaultConfig({ {}, Keycode::KEY_Unknown });
    ui->setStartConfig(mod->getKeybind());
    ui->show();
}

void ModuleInfoAlert::onChangeShortcut(CCObject* sender)
{
    auto ui = SetupShortcutUI::create([this](bool enabled, ModuleShortcutConfig conf)
    {
        mod->setShortcutConfig(enabled, conf);
    });

    ui->modID = mod->getID();
    ui->setStartConfig(mod->isShortcutEnabled(), mod->getShortcutConfig());
    ui->show();
}