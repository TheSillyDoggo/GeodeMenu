#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        PauseLayer::customSetup();

        if (!Module::get("pause-quests")->enabled)
            return;

        auto menu = CCMenu::create();
        menu->setScale(0.75f);
        menu->setAnchorPoint(ccp(0, 0));

        auto questsSpr = CCSprite::create("questsBtn.png"_spr);
        questsSpr->setScale(CCDirector::get()->getContentScaleFactor() / 4);

        auto questsBtn = CCMenuItemSpriteExtra::create(questsSpr, this, menu_selector(CreatorLayer::onChallenge));

        menu->setPosition(ccp(CCDirector::get()->getWinSize().width - 50, 50));

        if (this->getChildByID("dankmeme.globed2/playerlist-menu"))
            menu->setPositionY(110);

        menu->addChild(questsBtn);

        menu->setID("quests-menu"_spr);
        this->addChild(menu);
    }
};