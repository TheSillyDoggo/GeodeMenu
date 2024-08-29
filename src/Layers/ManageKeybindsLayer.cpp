#include "ManageKeybindsLayer.hpp"
#include "../Keybinds/RecordKeyStruct.hpp"
#include "../Client/Windows/IconEffects.hpp"

#define CELL_HEIGHT 25

ManageKeybindsLayer* ManageKeybindsLayer::create()
{
    ManageKeybindsLayer* pRet = new ManageKeybindsLayer();
    
    if (pRet && pRet->initWithSizeAndName(ccp(320, 260), "Keybinds"))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ManageKeybindsLayer::customSetup()
{
    auto scroll = ScrollLayer::create(ccp(250, 170));
    scroll->setPosition(size / 2 - scroll->getContentSize() / 2 + ccp(0, 6));

    auto bar = Scrollbar::create(scroll);

    auto border = geode::ListBorders::create();
    border->setContentSize(scroll->getContentSize() + ccp(0, -2));
    border->setZOrder(69);
    border->setSpriteFrames("geode.loader/geode-list-top.png", "geode.loader/geode-list-side.png", 2.25f);
    
    for (auto child : CCArrayExt<CCNodeRGBA*>(border->getChildren()))
    {
        child->setColor(ccc3(0, 0, 0));
        //child->setOpacity(100);
    }

    baseLayer->addChildAtPosition(border, Anchor::Center, ccp(0, 6));
    
    int count = 0;

    for (auto window : Client::get()->windows)
    {
        if (typeinfo_cast<IconEffects*>(window))
            continue;

        count++;

        for (auto module : window->modules)
        {
            if (typeinfo_cast<SliderModule*>(module) || typeinfo_cast<InputModule*>(module))
                continue;

            if (typeinfo_cast<Module*>(module))
                count++;
        }
    }

    scroll->m_contentLayer->setContentHeight(count * CELL_HEIGHT);
    scroll->moveToTop();

    int i = 0;

    for (auto window : Client::get()->windows)
    {
        if (typeinfo_cast<IconEffects*>(window))
            continue;

        auto bar = CCLayerColor::create(ccc4(0, 0, 0, 125));
        bar->setAnchorPoint(ccp(0, 1));
        bar->ignoreAnchorPointForPosition(false);
        bar->setPositionY(CELL_HEIGHT * count - (CELL_HEIGHT * i));
        bar->setContentSize(ccp(scroll->getContentWidth(), CELL_HEIGHT));

        auto label = CCLabelBMFont::create(window->name.c_str(), "goldFont.fnt");
        label->setAnchorPoint(ccp(0, 0.5f));
        label->setPosition(ccp(7.5f, CELL_HEIGHT / 2));
        label->limitLabelWidth(100, 0.6f, 0);
        //label->setOpacity(175);
        bar->addChild(label);

        scroll->m_contentLayer->addChild(bar);

        i++;
        int e = 0;
        for (auto module : window->modules)
        {
            if (typeinfo_cast<SliderModule*>(module) || typeinfo_cast<InputModule*>(module))
                continue;

            if (typeinfo_cast<Module*>(module))
            {
                auto bar = CCLayerColor::create(ccc4(0, 0, 0, !(e % 2) ? 25 : 75));
                bar->setAnchorPoint(ccp(0, 1));
                bar->ignoreAnchorPointForPosition(false);
                bar->setPositionY(CELL_HEIGHT * count - (CELL_HEIGHT * i));
                bar->setContentSize(ccp(scroll->getContentWidth(), CELL_HEIGHT));

                auto label = CCLabelBMFont::create(module->name.c_str(), "bigFont.fnt");
                label->setAnchorPoint(ccp(0, 0.5f));
                label->setPosition(ccp(7.5f, CELL_HEIGHT / 2));
                label->limitLabelWidth(130, 0.45f, 0);
                label->setOpacity(225);

                auto setSpr = ButtonSprite::create("Set", 100, 0, 1.0f, false);
                setSpr->setScale(0.6f);

                auto bnd = CCLabelBMFont::create(module->keybind.toString().c_str(), "chatFont.fnt");
                bnd->setAnchorPoint(ccp(1, 0.5f));
                bnd->setPositionX(-3);
                bnd->limitLabelWidth(70, 1, 0);

                auto del = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"), this, menu_selector(ManageKeybindsLayer::onDelete));
                del->getNormalImage()->setScale(0.7f);
                del->setPositionX(10);
                del->setVisible(module->keybind.key != enumKeyCodes::KEY_Unknown);
                del->setUserData(module);

                auto set = CCMenuItemSpriteExtra::create(setSpr, this, menu_selector(ManageKeybindsLayer::onSet));
                set->setUserData(module);
                set->setUserObject("delete", del);
                set->setUserObject("label", bnd);
                set->setVisible(module->keybind.key == enumKeyCodes::KEY_Unknown);

                del->setUserObject("set", set);
                del->setUserObject("label", bnd);

                auto menu = CCMenu::create();
                menu->addChild(set);
                menu->addChild(del);
                menu->addChild(bnd);

                bar->addChild(label);
                bar->addChildAtPosition(menu, Anchor::Right, ccp(-25, 0));
                scroll->m_contentLayer->addChild(bar);
                i++;
                e++;
            }
        }
    }

    baseLayer->addChild(scroll);
    baseLayer->addChildAtPosition(bar, Anchor::Right, ccp(-18.5f, 6));
}

void ManageKeybindsLayer::onSet(CCObject* sender)
{
    auto popup = RecordKeyStruct::create([this, sender](KeyStruct key){
        auto mod = as<Module*>(as<CCNode*>(sender)->getUserData());

        mod->keybind = key;
        mod->save();

        as<CCNode*>(sender)->setVisible(false);
        as<CCNode*>(as<CCNode*>(sender)->getUserObject("delete"))->setVisible(true);
        as<CCNode*>(as<CCNode*>(sender)->getUserObject("label"))->setVisible(true);
        as<CCLabelBMFont*>(as<CCNode*>(sender)->getUserObject("label"))->setString(key.toString().c_str());
        as<CCLabelBMFont*>(as<CCNode*>(sender)->getUserObject("label"))->limitLabelWidth(70, 1, 0);
    });

    CCScene::get()->addChild(popup, this->getZOrder() + 1);
}

void ManageKeybindsLayer::onDelete(CCObject* sender)
{
    auto popup = geode::createQuickPopup(as<Module*>(as<CCNode*>(sender)->getUserData())->name.c_str(), "Are you sure you want to <cr>delete</c>\nthis bind?", "Cancel", "Delete", [this, sender](FLAlertLayer*, bool right){
        if (right)
        {
            as<Module*>(as<CCNode*>(sender)->getUserData())->keybind = KeyStruct();
            as<Module*>(as<CCNode*>(sender)->getUserData())->save();
            as<CCNode*>(sender)->setVisible(false);
            as<CCNode*>(as<CCNode*>(sender)->getUserObject("label"))->setVisible(false);
            as<CCNode*>(as<CCNode*>(sender)->getUserObject("set"))->setVisible(true);
        }
    });

    popup->m_button2->updateBGImage("GJ_button_06.png");
}