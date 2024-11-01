#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameStatsManager)
{
    void incrementStat(char const* p0, int p1)
    {
        if (p0 == "2")
            return;

        GameStatsManager::incrementStat(p0, p1);
    }

    QOLMOD_MOD_ALL_HOOKS("freeze-attempts")
};

class $modify (PlayLayer)
{
    void resetLevel()
    {
        auto atts = m_level->m_attempts;

        PlayLayer::resetLevel();

        m_level->m_attempts = atts;
    }

    QOLMOD_MOD_ALL_HOOKS("freeze-attempts")
};