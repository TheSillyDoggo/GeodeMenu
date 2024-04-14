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

class $modify (GameStatsManager)
{
    bool hasCompletedMainLevel(int levelID)
    {
        if (Client::GetModuleEnabled("tower-level-bypass") && levelID >= 5000 && levelID <= 5004)
            return true;

        return GameStatsManager::hasCompletedMainLevel(levelID);
    }
};
