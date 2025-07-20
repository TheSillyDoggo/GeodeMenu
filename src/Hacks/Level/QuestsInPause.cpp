#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class QuestsInPause : public Module
{
    public:
        MODULE_SETUP(QuestsInPause)
        {
            setName("Quests In Pause");
            setID("pause-quests");
            setCategory("Level");
            setDescription("Adds a button to open the quests menu in the pause menu");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(QuestsInPause);

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        PauseLayer::customSetup();

        if (!QuestsInPause::get()->getRealEnabled())
            return;

        auto menu = CCMenu::create();
        menu->setScale(0.75f);
        menu->setAnchorPoint(ccp(0, 0));

        auto questsSpr = CCSprite::create("questsBtn.png"_spr);

        auto questsBtn = CCMenuItemSpriteExtra::create(questsSpr, this, menu_selector(CreatorLayer::onChallenge));

        menu->setPosition(ccp(CCDirector::get()->getWinSize().width - 50, 50));

        if (this->getChildByID("dankmeme.globed2/playerlist-menu"))
            menu->setPositionY(110);

        menu->addChild(questsBtn);

        menu->setID("quests-menu"_spr);
        this->addChild(menu);
    }
};