/*#include "../../Client/Module.hpp"
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;

class AutoClaimQuests : public Module
{
    public:
        MODULE_SETUP(AutoClaimQuests)
        {
            setName("Auto Claim Quests");
            setID("auto-claim-quests");
            setCategory("Universal");
            setDescription("Automatically claims a quest after it's completed");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(AutoClaimQuests)

class $modify (GameStatsManager)
{
    void incrementChallenge(GJChallengeType type, int amount)
    {
        GameStatsManager::incrementChallenge(type, amount);

        if (AutoClaimQuests::get()->getRealEnabled())
        {
            for (size_t i = 1; i < 3 + 1; i++)
            {
                if (auto chal = getChallenge(i))
                {
                    if (chal->m_canClaim)
                    {
                        auto page = ChallengesPage::create();

                        page->claimItem(static_cast<ChallengeNode*>(page->m_challengeNodes->objectForKey(i)), chal, ccp(0, 0));

                        page->removeFromParent();
                    }
                }
            }
        }
    }
};*/