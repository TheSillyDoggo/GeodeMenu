#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/CharacterColorPage.hpp>

using namespace geode::prelude;

class IconKitInPause : public Module
{
    public:
        MODULE_SETUP(IconKitInPause)
        {
            setName("Icon Kit in Pause");
            setID("pause-icon-kit");
            setCategory("Level");
            setDescription("asdf");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(IconKitInPause);

// TODO: Fix colours not saving after a player dies :c

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        PauseLayer::customSetup();

        if (IconKitInPause::get()->getRealEnabled())
        {
            auto clip = CCNode::create();// BoundingBoxClipNode::create();
            clip->setPosition(CCDirector::get()->getWinSize() + ccp(-10, -10));
            clip->setAnchorPoint(ccp(1, 1));
            clip->setContentSize(ccp(120, 120));

            auto menu = CCMenu::create();
            menu->setPosition(ccp(45, clip->getContentHeight()));
            menu->setContentSize(ccp(0, 0));

            auto spr = CCSprite::createWithSpriteFrameName("garageRope_001.png");

            auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(LevelInfoLayer::onGarage));
            btn->setPositionY(-27);
            btn->m_startPosition = spr->getContentSize() / 2;
            btn->m_offset = ccp(0, -8);
            btn->m_animationType = MenuAnimationType::Move;

            menu->addChild(btn);
            clip->addChild(menu);

            this->addChild(clip, 420);
        }
    }
};

class $modify (CCLayer)
{
    virtual void onEnter()
    {
        if (auto pl = typeinfo_cast<PlayLayer*>(this))
        {
            if (pl->m_isPaused)
                return;
        }

        CCLayer::onEnter();
    }
};

class $modify (GJGarageLayer)
{
    void onSelect(cocos2d::CCObject* sender)
    {
        GJGarageLayer::onSelect(sender);

        int id = sender->getTag();

        if (auto pl = PlayLayer::get())
        {
            auto plr = pl->m_player1;

            if (auto sep = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
            {
                if (sep->getSavedValue<bool>("2pselected"))
                    plr = pl->m_player2;
            }

            if (!plr)
                return;

            if (m_selectedIconType == IconType::Cube)
                plr->updatePlayerFrame(id);

            if (m_selectedIconType == IconType::Ship)
                plr->updatePlayerShipFrame(id);

            if (m_selectedIconType == IconType::Ball)
                plr->updatePlayerRollFrame(id);

            if (m_selectedIconType == IconType::Ufo)
                plr->updatePlayerBirdFrame(id);

            if (m_selectedIconType == IconType::Wave)
                plr->updatePlayerDartFrame(id);

            if (m_selectedIconType == IconType::Robot)
                plr->updatePlayerRobotFrame(id);

            if (m_selectedIconType == IconType::Spider)
                plr->updatePlayerSpiderFrame(id);

            if (m_selectedIconType == IconType::Swing)
                plr->updatePlayerSwingFrame(id);

            if (m_selectedIconType == IconType::Jetpack)
                plr->updatePlayerJetpackFrame(id);
        }
    }
};

class $modify (CharacterColorPage)
{
    void onPlayerColor(cocos2d::CCObject* sender)
    {
        CharacterColorPage::onPlayerColor(sender);

        auto col = GameManager::get()->colorForIdx(sender->getTag());

        if (auto pl = PlayLayer::get())
        {
            auto plr = pl->m_player1;

            if (auto sep = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
            {
                if (sep->getSavedValue<bool>("2pselected"))
                    plr = pl->m_player2;
            }

            if (!plr)
                return;

            if (m_colorMode == 0)
            {
                plr->setColor(col);
                plr->m_playerColor1 = col;
            }

            if (m_colorMode == 1)
            {
                plr->setSecondColor(col);
                plr->m_playerColor2 = col;
            }

            if (m_colorMode == 2)
                plr->setGlowColor(col);
        }
    }

    void toggleGlow(cocos2d::CCObject* sender)
    {
        CharacterColorPage::toggleGlow(sender);

        if (auto pl = PlayLayer::get())
        {
            auto plr = pl->m_player1;

            if (auto sep = Loader::get()->getLoadedMod("weebify.separate_dual_icons"))
            {
                if (sep->getSavedValue<bool>("2pselected"))
                    plr = pl->m_player2;
            }

            if (!plr)
                return;

            plr->m_hasGlow = as<CCMenuItemToggler*>(sender)->isToggled();
            plr->updatePlayerGlow();
        }
    }
};