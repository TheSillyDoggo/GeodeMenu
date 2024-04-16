#include <Geode/Geode.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/LevelAreaInnerLayer.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (LevelPage)
{
    void onPlay(cocos2d::CCObject* sender)
    {
        auto v = m_level->m_requiredCoins;

        if (Client::GetModuleEnabled("main-level-bypass"))
            m_level->m_requiredCoins = 0;

        LevelPage::onPlay(sender);

        m_level->m_requiredCoins = v;
    }
};

class $modify (LevelAreaInnerLayer)
{
    bool init(bool p0)
    {
        if (!LevelAreaInnerLayer::init(p0))
            return false;

        auto v = getChildByTagRecursive(this, 5001);

        if (v)
        {
            CCArrayExt<CCMenuItemSpriteExtra*> objs = v->getParent()->getChildren();

            for (auto child : objs)
            {
                child->setEnabled(true);
            }
        }

        return true;
    }
};
