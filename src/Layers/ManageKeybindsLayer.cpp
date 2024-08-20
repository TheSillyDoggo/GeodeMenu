#include "ManageKeybindsLayer.hpp"
#include "../Keybinds/RecordKeyStruct.hpp"

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
    
    int count = 0;

    for (auto window : Client::get()->windows)
    {
        count++;

        for (auto module : window->modules)
        {
            if (typeinfo_cast<Module*>(module))
                count++;
        }
    }

    scroll->m_contentLayer->setContentHeight(count * CELL_HEIGHT);
    scroll->moveToTop();

    int i = 0;

    for (auto window : Client::get()->windows)
    {
        auto bar = CCLayerColor::create(ccc4(0, 0, 0, 125));
        bar->setAnchorPoint(ccp(0, 1));
        bar->ignoreAnchorPointForPosition(false);
        bar->setPositionY(CELL_HEIGHT * count - (CELL_HEIGHT * i));
        bar->setContentSize(ccp(scroll->getContentWidth(), CELL_HEIGHT));

        auto label = CCLabelBMFont::create(window->name.c_str(), "bigFont.fnt");
        label->setAnchorPoint(ccp(0, 0.5f));
        label->setPosition(ccp(7.5f, CELL_HEIGHT / 2));
        label->limitLabelWidth(100, 0.6f, 0);
        label->setOpacity(175);
        bar->addChild(label);

        scroll->m_contentLayer->addChild(bar);

        i++;
        int e = 0;
        for (auto module : window->modules)
        {
            if (dynamic_cast<Module*>(module))
            {
                auto bar = CCLayerColor::create(ccc4(0, 0, 0, !(e % 2) ? 25 : 75));
                bar->setAnchorPoint(ccp(0, 1));
                bar->ignoreAnchorPointForPosition(false);
                bar->setPositionY(CELL_HEIGHT * count - (CELL_HEIGHT * i));
                bar->setContentSize(ccp(scroll->getContentWidth(), CELL_HEIGHT));

                auto label = CCLabelBMFont::create(module->name.c_str(), "bigFont.fnt");
                label->setAnchorPoint(ccp(0, 0.5f));
                label->setPosition(ccp(7.5f, CELL_HEIGHT / 2));
                label->limitLabelWidth(170, 0.45f, 0);
                label->setOpacity(225);

                auto setSpr = ButtonSprite::create("Set", 100, 0, 1.0f, false);
                setSpr->setScale(0.6f);

                auto set = CCMenuItemSpriteExtra::create(setSpr, this, menu_selector(ManageKeybindsLayer::onSet));
                set->setUserData(module);

                auto menu = CCMenu::create();
                menu->addChild(set);

                bar->addChild(label);
                bar->addChildAtPosition(menu, Anchor::Right, ccp(-25, 0));
                scroll->m_contentLayer->addChild(bar);
                i++;
                e++;
            }
        }
    }

    baseLayer->addChild(scroll);
}

void ManageKeybindsLayer::onSet(CCObject* sender)
{
    auto popup = RecordKeyStruct::create([this, sender](KeyStruct key){
        log::info("key: {}", CCKeyboardDispatcher::get()->keyToString(key.key));
    });

    this->addChild(popup, 69);
}