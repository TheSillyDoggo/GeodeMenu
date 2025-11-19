/*
#include "../../Client/Module.hpp"
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

class AutoClaimDailyChests : public Module
{
    public:
        MODULE_SETUP(AutoClaimDailyChests)
        {
            setName("Auto Claim Daily Chests");
            setID("auto-claim-daily-chests");
            setCategory("Universal");
            setDescription("Automatically claims the daily chests when they are available");
            setDefaultEnabled(true);
        }

        void updateDailyChests(float dt)
        {
            if (AutoClaimDailyChests::get()->getRealEnabled())
            {
                auto rewards = GameStatsManager::sharedState()->m_rewardItems;

                // ありがとう
                // https://github.com/dotnetx86/Chest-Alert/blob/main/src/main.cpp
                if (rewards->count() != 0)
                {
                    int time1_value = 0x1;
                    int time2_value = 0x2;
                    GJRewardItem* time1 = typeinfo_cast<GJRewardItem*>(rewards->objectForKey(time1_value));
                    GJRewardItem* time2 = typeinfo_cast<GJRewardItem*>(rewards->objectForKey(time2_value));

                    if (time1 && time1->m_timeRemaining == 0)
                    {
                        log::info("claim 1: {}", time1->m_timeRemaining);
                    }

                    if (time2 && time2->m_timeRemaining == 0)
                    {
                        log::info("claim 2: {}", time2->m_timeRemaining);
                    }
                }
            }
        }
};

SUBMIT_HACK(AutoClaimDailyChests)

$on_mod(Loaded)
{
    // loads daily rewards
	GameLevelManager::sharedState()->getGJRewards(0);

    CCScheduler::get()->scheduleSelector(schedule_selector(AutoClaimDailyChests::updateDailyChests), CCNode::create(), 5, false);
}
*/