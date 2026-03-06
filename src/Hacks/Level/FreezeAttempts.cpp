#include "../../Client/InputModule.hpp"
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class FreezeAttempts : public Module
{
    public:
        MODULE_SETUP(FreezeAttempts)
        {
            setName("Freeze Attempts");
            setID("freeze-attempts");
            setCategory("Level");
            setDescription("Freezes your attempt count stat and on level");
        }
};

SUBMIT_HACK(FreezeAttempts);

class $modify (GameStatsManager)
{
    void incrementStat(char const* p0, int p1)
    {
        if (FreezeAttempts::get()->getRealEnabled())
        {
            if (strcmp(p0, "2") == 0)
                return;
        }

        GameStatsManager::incrementStat(p0, p1);
    }
};

class $modify (PlayLayer)
{
    void resetLevel()
    {
        auto atts = m_level->m_attempts;

        PlayLayer::resetLevel();

        if (FreezeAttempts::get()->getRealEnabled())
            m_level->m_attempts = atts;
    }
};