#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/NumberInputLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(LevelInfoLayer)
{
    bool init(GJGameLevel *p0, bool p1)
    {
        if (!LevelInfoLayer::init(p0, p1))
            return false;

        if (auto menu = getChildByID("left-side-menu"))
        {
            if (auto locked = typeinfo_cast<CCMenuItemSpriteExtra*>(getChildBySpriteFrameName(menu, "GJ_duplicateLockedBtn_001.png")); locked && locked->isVisible())
            {
                locked->m_pfnSelector = menu_selector(LevelInfoLayer::confirmClone);
                locked->setSprite(CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png"));

                return true;
            }

            if (auto unlocked = typeinfo_cast<CCMenuItemSpriteExtra*>(getChildBySpriteFrameName(menu, "GJ_duplicateBtn_001.png")))
            {
                unlocked->setVisible(true);
            }
            else
            {
                auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png"), this, menu_selector(LevelInfoLayer::confirmClone));
                btn->setID("copy-button");

                m_cloneBtn = btn;

                menu->addChild(btn);
                menu->updateLayout();
                return true;
            }
        }

        // fallback to bad code if node ids isn't installed

        auto gm = GameManager::sharedState();
        if (gm->m_playerUserID == p0->m_userID) return true;
        if (m_cloneBtn == nullptr) return true;

        auto aCloneBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_duplicateBtn_001.png"), this, menu_selector(LevelInfoLayer::confirmClone));
        aCloneBtn->setPosition(m_cloneBtn->getPosition());
        m_cloneBtn->getParent()->addChild(aCloneBtn);
        m_cloneBtn->setVisible(false);
        
        return true;
    }

    QOLMOD_MOD_HOOK("copy-hack", "LevelInfoLayer::init")
};

class $modify (ShowPasswordLayer, NumberInputLayer)
{
    void onShowPassword(CCObject* sender)
    {
        auto v = as<CCNode*>(sender)->getTag();

        FLAlertLayer::create("Level Password", fmt::format("The Level Password is <cl>{}</c>.", fmt::format("{}", v).substr(1)), "Thanks")->show();
    }

    virtual bool init()
    {
        if (!NumberInputLayer::init())
            return false;

        if (!Client::GetModuleEnabled("show-password"))
            return true;

        if (auto l = getChildOfType<LevelInfoLayer>(CCScene::get(), 0))
        {
            auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, menu_selector(ShowPasswordLayer::onShowPassword));
            btn->setTag(l->m_level->m_password.value());
            btn->setPosition(ccp(119, 117));

            getChildOfType<CCMenu>(m_mainLayer, 0)->addChild(btn);
        }

        return true;
    }
};