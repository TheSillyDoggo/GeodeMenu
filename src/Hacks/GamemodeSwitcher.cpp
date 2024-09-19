#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Layers/SwitchGamemodePopup.hpp"
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (SwitchPauseLayer, PauseLayer)
{
    void onSwitch(CCObject* sender)
    {
        SwitchGamemodePopup::addToScene();
    }

    void addNode()
    {
        CCNode* menu = getChildByID("bottom-button-menu");

        if (!menu)
        {
            auto bottomMenu = CCMenu::create();
            bottomMenu->setLayout(
                RowLayout::create()
                    ->setGap(4.f)
                    ->setAxisAlignment(AxisAlignment::Center)
                    ->setCrossAxisOverflow(false)
            );
            bottomMenu->setID("bottom-button-menu");
            bottomMenu->setPosition({(CCDirector::get()->getWinSize().width / 2), 30.f});
            bottomMenu->setContentSize({400, 30});
            bottomMenu->setZOrder(10);
            this->addChild(bottomMenu);

            menu = bottomMenu;
        }

        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_iconBtn_on_001.png"), this, menu_selector(SwitchPauseLayer::onSwitch));
        menu->addChild(btn);
        menu->updateLayout();
    }

    virtual void customSetup()
    {
        PauseLayer::customSetup();

        addNode();
    }

    QOLMOD_MOD_HOOK("gamemode-switcher", "PauseLayer::customSetup")
};

$execute
{
    Loader::get()->queueInMainThread([] {
        Client::GetModule("gamemode-switcher")->onToggle = [](bool enabled){
            if (auto p = as<SwitchPauseLayer*>(getChildOfType<PauseLayer>(CCScene::get(), 0)))
            {
                if (enabled)
                    p->addNode();
                    
            }
        };
    });
}